#include <ghc/filesystem.hpp>

#include "QueryCache.hpp"
#include "utils/patches.hpp"

using namespace rack;

std::string QueryCache::getCacheFilename() {
    return asset::user("prefabs/" + this->name + ".json");
}

std::string QueryCache::getPatchFilename(NewPatchInfo patchInfo, PatchFile fileInfo) {
    return asset::user("patch-storage/" + patchInfo.author.slug + "/" + patchInfo.slug + ".vcv");
}

json_t* QueryCache::readCacheFile() {
    auto cacheFile = this->getCacheFilename();
    if (!system::exists(cacheFile)) {
        return nullptr;
    }

    FILE* file = fopen(cacheFile.c_str(), "r");
    if (!file) {
        DINFO("[Prefabs] When loading Settings, could not open %s", cacheFile.c_str());
        return nullptr;
    }

    json_error_t error;
    json_t* rootJ = json_loadf(file, 0, &error);
    if (!rootJ) {
        if (settings::devMode) {
            DINFO("[Prefabs] Failed to load cache. JSON parsing error at %s %d:%d %s",
                error.source,
                error.line,
                error.column,
                error.text);
        }
        return nullptr;
    }

    fclose(file);
    return rootJ;
}

std::vector<NewPatchInfo> QueryCache::loadCache() {
    auto rootJ = this->readCacheFile();
    if (!rootJ) {
        return {};
    }

    std::vector<NewPatchInfo> patches;

    for (int i = 0; i < json_array_size(rootJ); i++) {
        json_t* patchJ = json_array_get(rootJ, i);
        NewPatchInfo patch;
        patch.fromJson(patchJ);
        patches.push_back(patch);
    }

    return patches;
}

void QueryCache::saveCache() {
    auto path = this->getCacheFilename();
    auto parentPath = system::getDirectory(path);
    system::createDirectories(parentPath);

    FILE* file = fopen(path.c_str(), "w");
    if (!file) {
        DINFO("[Prefabs] When saving Settings, could not open %s", path.c_str());
        return;
    }

    // sort this->cache based on patch id
    std::sort(this->cache.begin(), this->cache.end(), [](NewPatchInfo a, NewPatchInfo b) { return a.id < b.id; });

    json_t* rootJ = json_array();
    for (auto patch : this->cache) {
        json_array_append_new(rootJ, patch.toJson());
    }

    json_dumpf(rootJ, file, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
    fclose(file);
    json_decref(rootJ);

    DINFO("[Prefabs] Saved cache to %s", path.c_str());
}

std::vector<NewPatchInfo> QueryCache::getNewPatches() {
    if (!this->queryFunc) {
        QINFO("No query function set for %s", this->name.c_str());
        return {};
    }

    return this->queryFunc(this->client, this->cache.size());
}

void QueryCache::refresh() {
    // this calls getNewPatches, and for each new patch, it calls downloadPatch
    auto newPatches = this->getNewPatches();

    for (auto patch : newPatches) {
        this->cache.push_back(patch);
    }

    this->saveCache();
    this->downloadAll();
}

void QueryCache::downloadAll() {
    for (auto patch : this->cache) {
        this->downloadPatch(patch);
    }
}

void QueryCache::downloadPatch(NewPatchInfo patchInfo) {
    for (auto fileInfo : patchInfo.files) {
        auto extension = system::getExtension(fileInfo.filename);

        if (extension != ".vcv") {
            continue;
        }

        this->downloadFile(patchInfo, fileInfo);
    }
}

bool QueryCache::updateMetadata(NewPatchInfo patchInfo, std::string patchFile) {
    auto inFile = fopen(patchFile.c_str(), "r");

    if (!inFile) {
        DINFO("[Prefabs] When loading patch inFile, could not open %s", patchFile.c_str());
        return false;
    }

    json_error_t error;
    json_t* rootJ = json_loadf(inFile, 0, &error);

    fclose(inFile);

    if (!rootJ) {
        DINFO("[Prefabs] Failed to load patch file. JSON parsing error at %s %d:%d %s",
            error.source,
            error.line,
            error.column,
            error.text);
        return false;
    }

    json_object_set_new(rootJ, "metadata", patchInfo.toJson());

    auto outFile = fopen(patchFile.c_str(), "w");
    json_dumpf(rootJ, outFile, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
    fclose(outFile);
    json_decref(rootJ);

    return true;
}

bool QueryCache::updateArchive(NewPatchInfo patchInfo, std::string archiveFile, std::string destinationFile) {
    auto archiveFolder = archiveFile + "-unpacked";
    system::unarchiveToDirectory(archiveFile, archiveFolder);
    auto patchPath = archiveFolder + "/patch.json";
    updateMetadata(patchInfo, patchPath);
    system::archiveDirectory(archiveFile, archiveFolder, 1);
    system::removeRecursively(archiveFolder);
    return true;
}

void QueryCache::downloadFile(NewPatchInfo patchInfo, PatchFile fileInfo) {
    auto path = this->getPatchFilename(patchInfo, fileInfo);

    if (system::exists(path)) {
        return;
    }

    auto stagingPath = asset::user("prefabs/staging/");
    system::createDirectories(stagingPath);

    std::string randomFilename = "";
    for (int i = 0; i < 10; i++) {
        randomFilename += (char)('a' + (rand() % 26));
    }

    auto tempPath = stagingPath + randomFilename;

    if (!this->client.downloadPatch(fileInfo.url, tempPath)) {
        DINFO("[Prefabs] Failed to download patch %s", fileInfo.url.c_str());
        return;
    }

    bool isLegacy = isPatchLegacyV1(tempPath);

    if (!isLegacy) {
        updateArchive(patchInfo, tempPath, path);
    } else {
        updateMetadata(patchInfo, tempPath);
    }

    auto dirname = system::getDirectory(path);
    system::createDirectories(dirname);

    DINFO("Copying %s to %s", tempPath.c_str(), path.c_str());
    system::copy(tempPath, path);
    system::remove(tempPath);
}

QueryCache::QueryCache(std::string name, StorageClient client) : name(name), client(client) {
    this->cache = this->loadCache();
}
