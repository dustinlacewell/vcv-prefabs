#pragma once

#include <rack.hpp>

#include "ModuleSorter.hpp"
#include "ModuleTagManager.hpp"
#include "SimpleQuantity.hpp"
#include "modules/prefabs/index/Index.hpp"
#include "modules/prefabs/index/StorageIndex.hpp"
#include "modules/prefabs/sources/ThreadedStore.hpp"

using namespace rack;

typedef std::tuple<std::string, int> StorageUser;
typedef std::tuple<std::string, int> StoragePatch;

// LayoutType is an enum with STACKED and TABBED
enum LayoutType { STACKED, TABBED };

struct State {
    bool showing = true;
    Vec pos = Vec(30, 30);

    bool browserMode;
    bool monoIcon;
    LayoutType layout;

    ModuleTagManager tagManager;
    ModuleSorter moduleSorter;

    ThreadedStore store;

    Index prefabsIndex;
    Index patchesIndex;
    StorageIndex storageIndex;

    SimpleQuantity searchResultsQuantity;
    SimpleQuantity colorQuantity;
    SimpleQuantity discoSpeedQuantity;

    std::set<std::tuple<std::string, std::string>> extraPrefabSources;
    std::set<std::tuple<std::string, std::string>> extraPatchSources;

    std::string storageUsername;
    std::string storagePassword;
    std::vector<StorageUser> storageUsers;

    State();

    json_t* toJson();
    void fromJson(json_t* rootJ);

    void reload();
    void save();
    void load();
    void refresh();
};