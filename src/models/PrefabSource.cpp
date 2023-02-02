#include "PrefabSource.hpp"
#include <plugin.hpp>
#include <rack.hpp>
#include "Prefab.hpp"
#include "utils/files.hpp"

using namespace rack;

PrefabSource::PrefabSource()
{
    slug = "";
    root = "";
    prefabs = set<Prefab>();
    tags = std::map<std::string, std::set<Prefab>>();
    plugins = std::map<std::string, std::map<std::string, std::set<Prefab>>>();
}

PrefabSource::PrefabSource(std::string slug, std::string root)
{
    this->slug = slug;
    this->root = root;
    prefabs = set<Prefab>();
    tags = std::map<std::string, std::set<Prefab>>();
    plugins = std::map<std::string, std::map<std::string, std::set<Prefab>>>();
}

std::string PrefabSource::pathForTag(std::string tagName)
{
    return root + (tagName.empty() ? "" : ('/' + tagName));
}

std::string PrefabSource::pathForPrefab(std::string tagName, std::string prefabName)
{
    return pathForTag(tagName) + '/' + prefabName;
}

void PrefabSource::addPrefab(Prefab prefab)
{
    // flat prefab list
    prefabs.insert(prefab);

    // by-tag index
    for (auto tag : prefab.tags) {
        tags[tag].insert(prefab);
    }

    // by-plugin index
    for (auto [pluginName, pluginModules] : prefab.usedPlugins) {
        for (auto [moduleName, useCount] : pluginModules) {
            plugins[pluginName][moduleName].insert(prefab);
        }
    }
}

json_t* PrefabSource::loadFile(std::string path)
{
    std::string extension = extensionFrom(path);

    if (extension != "vcvs") {
        return nullptr;
    }

    json_error_t error;
    json_t* rootJ = json_load_file(path.c_str(), 0, &error);

    if (!rootJ) {
        return nullptr;
    }

    return rootJ;
}

bool PrefabSource::loadPrefab(std::string tagName, std::string prefabName)
{
    std::string prefab_path = pathForPrefab(tagName, prefabName);

    if (!isRegularFile(prefab_path)) {
        return false;
    }

    json_t* rootJ = loadFile(prefab_path);

    if (!rootJ) {
        return false;
    }

    // load prefab from json
    Prefab prefab = Prefab(prefab_path);
    prefab.addTag(tagName == "" ? "untagged" : tagName);
    prefab.fromJson(rootJ);

    prefab.source = slug;

    json_decref(rootJ);

    addPrefab(prefab);
    return true;
}

int PrefabSource::crawlTag(std::string tagName)
{
    int nPrefabs = 0;

    std::string tagPath = pathForTag(tagName);

    if (!isDirectory(tagPath)) {
        return 0;
    }

    eachDir(tagPath, [&](auto ent) {
        if (isDirectory(tagPath + '/' + ent->d_name)) {
            return;
        }
        if (loadPrefab(tagName, ent->d_name)) {
            nPrefabs++;
        }
    });

    return nPrefabs;
}

void PrefabSource::refresh()
{
    int nPrefabs = 0;

    tags.clear();
    plugins.clear();
    prefabs.clear();

    nPrefabs += this->crawlTag("");  // untagged prefabs

    eachDir(root, [&nPrefabs, this](auto ent) {
        // check if ent is a directory
        if (!isDirectory(root + '/' + ent->d_name)) {
            return;
        }
        nPrefabs += this->crawlTag(ent->d_name);
    });

    total = nPrefabs;
}
