#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Prefab.hpp"

typedef std::set<Prefab> PrefabSet;
typedef std::map<std::string, PrefabSet> TagMap;
typedef std::map<std::string, PrefabSet> ModuleMap;
typedef std::map<std::string, ModuleMap> PluginMap;

struct PrefabSource
{
    int total = 0;
    Plugin* plugin;

    PrefabSet prefabs;
    TagMap tags;
    PluginMap plugins;

    PrefabSource();
    PrefabSource(Plugin* plugin);
    std::string rootPath();
    std::string pathForTag(std::string tagName);
    std::string pathForPrefab(std::string tagName, std::string prefabName);

    void refresh();
    void addPrefab(Prefab prefab);

    int crawlTag(std::string tagName);
    bool loadPrefab(std::string tagName, std::string prefabName);
};