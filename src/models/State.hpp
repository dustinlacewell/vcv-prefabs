#pragma once

#include <rack.hpp>

#include "index/Index.hpp"
#include "models/ModuleSorter.hpp"
#include "models/ModuleTagManager.hpp"
#include "models/SimpleQuantity.hpp"
#include "sources/ThreadedStore.hpp"

using namespace rack;

struct State
{
    bool showing = true;
    Vec pos = Vec(30, 30);

    bool browserMode;

    ModuleTagManager tagManager;
    ModuleSorter moduleSorter;

    ThreadedStore store;

    Index prefabsIndex;
    Index patchesIndex;

    SimpleQuantity searchResultsQuantity;
    SimpleQuantity colorQuantity;
    SimpleQuantity discoSpeedQuantity;

    std::set<std::tuple<std::string, std::string>> extraPrefabSources;
    std::set<std::tuple<std::string, std::string>> extraPatchSources;

    State();

    json_t* toJson();
    void fromJson(json_t* rootJ);

    void refresh();
    void save();
    void load();
};