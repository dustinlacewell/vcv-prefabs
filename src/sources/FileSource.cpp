#include <plugin.hpp>
#include <rack.hpp>

#include "FileSource.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"

using namespace rack;

FileSource::FileSource(std::string slug, std::string path) : Source(), slug(slug), path(path) {}

FileSource::~FileSource() {}

int FileSource::getTotal()
{
    return total;
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
    std::string rack_path = pathForItem(tagName, rackName);

    if (!isRegularFile(rack_path)) {
        return nullptr;
    }

    json_t* rootJ = readJson(rack_path);

    if (!rootJ) {
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
        auto rack = read(tagName, ent->d_name);
        if (rack != nullptr) {
            nRacks++;
            DINFO("[Prefabs] Loaded rack %s", rack->slug.c_str());
            if (callback) {
                callback(*rack);
            }
        }
    });

    return nRacks;
}

void FileSource::refresh()
{
    total = 0;

    racks.clear();

    total += this->crawlTag("");  // ungrouped racks

    eachDir(path, [this](auto ent) {
        // check if ent is a directory
        if (!isDirectory(path + '/' + ent->d_name)) {
            return;
        }
        total += crawlTag(ent->d_name);
    });
}