#pragma once

#include <rack.hpp>
#include "models/ModuleSorter.hpp"
#include "models/ModuleTagManager.hpp"
#include "models/PatchStore.hpp"
#include "models/Prefab.hpp"
#include "models/PrefabStore.hpp"
#include "models/SimpleQuantity.hpp"

using namespace rack;

struct State
{
    bool showing = true;
    Vec pos = Vec(30, 30);

    bool refreshEveryTime;

    ModuleTagManager tagManager;
    ModuleSorter moduleSorter;

    PrefabStore prefabs;
    PatchStore patches;

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