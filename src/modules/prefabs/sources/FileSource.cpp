#include <ghc/filesystem.hpp>

#include "efsw/efsw.hpp"

#include <plugin.hpp>
#include <rack.hpp>
#include <thread>

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

    // replace all \ with /
    std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

    if (fs::is_directory(fullPath)) {
        return;
    }

    FileSource* that = (FileSource*)param;
    if (action == EFSW_ADD || action == EFSW_DELETE || action == EFSW_MODIFIED || action == EFSW_MOVED) {

        auto watcherEvent = eventString(action);

        if (watcherEvent == "MODIFIED") {
            SINFO("PATCH MODIFIED: %s", fullPath.c_str());
            that->readRack(fullPath);
        }
    }
}

void FileSource::createWatcher()
{
    SINFO("Creating patch watcher for %s", path.c_str());
    watcher = efsw_create(false);
    efsw_addwatch(watcher, path.c_str(), watchCallback, true, this);
    efsw_watch(watcher);
}

FileSource::FileSource(std::string slug, std::string path) : Source(), slug(slug), path(path)
{
    createWatcher();
}

FileSource::~FileSource()
{
    if (watcher != nullptr) {
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

bool endsWith(const std::string& A, const std::string& B)
{
    if (B.size() > A.size()) {
        return false;
    }
    return A.rfind(B) == (A.size() - B.size());
}

Rack* FileSource::read(std::string filename)
{
    if (!isRegularFile(filename)) {
        SINFO("FileSource::read() not a regular file: %s", filename.c_str());
        return nullptr;
    }

    auto groupname = findFirstChild(this->path, filename);

    // if filename ends with groupname
    // then groupname should be set to "untagged"
    if (endsWith(filename, groupname)) {
        groupname = "untagged";
    }

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
        SINFO("FileSource::crawlRoot() not a directory: %s", root.c_str());
        return 0;
    }

    auto rootFiles = allFiles(root);
    auto rackFiles = filterFiles(rootFiles);

    for (auto rackFile : rackFiles) {
        nRacks += readRack(rackFile);
    }

    return nRacks;
}

void FileSource::load()
{
    DINFO("[Prefabs] FileSource::load() %s", slug.c_str());

    racks.clear();

    this->crawlRoot(path);  // ungrouped racks

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