#include <ghc/filesystem.hpp>

#include "efsw/efsw.hpp"

#include <plugin.hpp>
#include <rack.hpp>

#include "FileSource.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"

namespace fs = ghc::filesystem;

using namespace rack;

static std::string eventString(efsw_action action)
{
    switch (action) {
        case EFSW_ADD:
            return "ADD";
        case EFSW_DELETE:
            return "DELETE";
        case EFSW_MODIFIED:
            return "MODIFIED";
        case EFSW_MOVED:
            return "MOVED";
    }
}

static std::string dirname(std::string path)
{
    return path.substr(path.find_last_of('\\') + 1);
}

static std::string snipTrailingSlash(std::string str)
{
    if (str.back() == '\\') {
        str.pop_back();
    }
    return str;
}

static void watchCallback(efsw_watcher watcher,
    efsw_watchid watchid,
    const char* dir,
    const char* filename,
    enum efsw_action action,
    const char* old_filename,
    void* param)
{
    auto fullPath = std::string(dir) + "/" + std::string(filename);

    // bail if is directory

    if (fs::is_directory(fullPath)) {
        return;
    }

    FileSource* that = (FileSource*)param;
    if (action == EFSW_ADD || action == EFSW_DELETE || action == EFSW_MODIFIED || action == EFSW_MOVED) {

        auto watcherEvent = eventString(action);

        // Check filename
        that->readRack(filename);
    }
}

FileSource::FileSource(std::string slug, std::string path) : Source(), slug(slug), path(path)
{
    watcher = efsw_create(false);
    efsw_addwatch(watcher, path.c_str(), watchCallback, true, this);
    efsw_watch(watcher);
}

FileSource::~FileSource()
{
    if (watcher) {
        efsw_release(watcher);
        watcher = NULL;
    }
}

int FileSource::getTotal()
{
    return racks.size();
}

std::string FileSource::getSlug()
{
    return slug;
}

std::string FileSource::getPath()
{
    return path;
}

void FileSource::setCallback(std::function<void(Rack)> newCallback)
{
    callback = newCallback;
}

//std::string FileSource::pathForTag(std::string tagName)
//{
//    return path + (tagName.empty() ? "" : ('/' + tagName));
//}
//
//std::string FileSource::pathForItem(std::string tagName, std::string prefabName)
//{
//    return pathForTag(tagName) + '/' + prefabName;
//}

json_t* FileSource::readJson(std::string filePath)
{
    std::string extension = extensionFrom(filePath);

    if (extension != "vcvs") {
        return nullptr;
    }

    json_error_t error;
    json_t* rootJ = json_load_file(filePath.c_str(), 0, &error);

    if (!rootJ) {
        return nullptr;
    }

    return rootJ;
}

Rack* FileSource::read(std::string filename)
{
    if (!isRegularFile(filename)) {
        SINFO("FileSource::read() not a regular file: %s", filename.c_str());
        return nullptr;
    }

    auto groupname = findFirstChild(this->path, filename);

    json_t* rootJ = readJson(filename);

    if (!rootJ) {
        SINFO("rootJ is null: %s", filename.c_str());
        return nullptr;
    }

    auto group = groupname == "" ? "ungrouped" : groupname;
    Rack rack = Rack(slug, group, filename);
    rack.fromJson(rootJ);

    rack.source = slug;

    json_decref(rootJ);

    racks.emplace(rack.slug, rack);
    return &racks.find(rack.slug)->second;
}

std::vector<std::string> FileSource::filterFiles(std::vector<std::string>& files)
{
    return filesWithExtension(files, "vcvs");
}

int FileSource::crawlRoot(std::string root)
{
    int nRacks = 0;

    if (!isDirectory(root)) {
        return 0;
    }

    auto rootFiles = allFiles(root);
    auto rackFiles = filterFiles(rootFiles);

    for (auto rackFile : rackFiles) {
        nRacks += readRack(rackFile);
    }

    return nRacks;
}

void FileSource::refresh()
{
    DINFO("[Prefabs] FileSource::refresh() %s", slug.c_str());

    racks.clear();

    this->crawlRoot("");  // ungrouped racks

    eachDir(path, [this](auto ent) {
        auto fullPath = path + '/' + ent->d_name;
        if (isDirectory(fullPath)) {
            crawlRoot(fullPath);
        }
    });
}

int FileSource::readRack(std::string filename)
{
    DINFO("[Prefabs] %s Loading rack %s", this->slug.c_str(), filename.c_str());

    auto rack = this->read(filename);

    if (rack != nullptr) {
        DINFO("[Prefabs] Loaded rack %s", rack->slug.c_str());
        if (this->callback) {
            this->callback(*rack);
        }
        return 0;
    }

    DINFO("[Prefabs] Failed to load rack %s", filename.c_str());
    return 0;
}