#include <filesystem>
#include <iostream>
#include <plugin.hpp>
#include <rack.hpp>

#include "FileSource.hpp"
#include "ThreadedStore.hpp"
#include "efsw/efsw.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"

using namespace rack;

static void watchCallback(efsw_watcher watcher,
    efsw_watchid watchid,
    const char* dir,
    const char* filename,
    enum efsw_action action,
    const char* old_filename,
    void* param)
{
    DINFO("FileSource::FileSource() old_filename: %s", old_filename);
    DINFO("FileSource::FileSource() callback: %d: %s / %s", action, dir, filename);
    auto fullPath = std::string(dir) + "/" + std::string(filename);

    // bail if is directory
    if (std::filesystem::is_directory(fullPath)) {
        return;
    }

    FileSource* that = (FileSource*)param;
    if (action == EFSW_ADD || action == EFSW_DELETE || action == EFSW_MODIFIED || action == EFSW_MOVED) {
        // snip trailing / using pop
        std::string dirStr = dir;
        if (dirStr.back() == '\\') {
            dirStr.pop_back();
        }

        // get dirname
        std::string dirName = dirStr.substr(dirStr.find_last_of('\\') + 1);

        // Check filename
        DINFO("FileSource::FileSource() callback: %s / %s", dirName.c_str(), filename);
        that->readRack(dirName, filename);
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
    DINFO("FileSource::~FileSource() %s", slug.c_str());
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

std::string FileSource::pathForTag(std::string tagName)
{
    return path + (tagName.empty() ? "" : ('/' + tagName));
}

std::string FileSource::pathForItem(std::string tagName, std::string prefabName)
{
    return pathForTag(tagName) + '/' + prefabName;
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

Rack* FileSource::read(std::string tagName, std::string rackName)
{
    DINFO("My slug is %s", slug.c_str());
    DINFO("My path is %s", path.c_str());
    DINFO("[Prefabs] FileSource::read() tagName: %s, rackName: %s", tagName.c_str(), rackName.c_str());
    std::string rack_path = path + (tagName.empty() ? "" : ('/' + tagName)) + '/' + rackName;

    DINFO("[Prefabs] FileSource::read() rack_path: %s", rack_path.c_str());

    //    if (!isRegularFile(rack_path)) {
    //        DINFO("[Prefabs] FileSource::read() rack_path is not a regular file: %s", rack_path.c_str());
    //        return nullptr;
    //    }

    json_t* rootJ = readJson(rack_path);

    if (!rootJ) {
        DINFO("[Prefabs] FileSource::read() rootJ is null: %s", rack_path.c_str());
        return nullptr;
    }

    auto group = tagName == "" ? "ungrouped" : tagName;
    Rack rack = Rack(slug, group, rack_path);
    rack.fromJson(rootJ);

    rack.source = slug;

    json_decref(rootJ);

    racks.emplace(rack.slug, rack);
    return &racks.find(rack.slug)->second;
}

int FileSource::crawlTag(std::string tagName)
{
    int nRacks = 0;

    std::string tagPath = pathForTag(tagName);

    if (!isDirectory(tagPath)) {
        return 0;
    }

    eachDir(tagPath, [&](auto ent) {
        if (isDirectory(tagPath + '/' + ent->d_name)) {
            return;
        }
        nRacks += readRack(tagName, ent->d_name);
    });

    return nRacks;
}

void FileSource::refresh()
{
    racks.clear();

    this->crawlTag("");  // ungrouped racks

    eachDir(path, [this](auto ent) {
        // check if ent is a directory
        if (!isDirectory(path + '/' + ent->d_name)) {
            return;
        }
        crawlTag(ent->d_name);
    });
}

int FileSource::readRack(std::string groupName, std::string rackName)
{
    DINFO("[Prefabs] Loading rack %s %s %s", this->slug.c_str(), groupName.c_str(), rackName.c_str());
    auto rack = this->read(groupName, rackName);

    if (rack != nullptr) {
        DINFO("[Prefabs] Loaded rack %s", rack->slug.c_str());
        if (this->callback) {
            this->callback(*rack);
        }
        return 0;
    }

    DINFO("[Prefabs] Failed to load rack %s", rackName.c_str());
    return 0;
}