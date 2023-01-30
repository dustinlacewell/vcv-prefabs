#pragma once

#include <rack.hpp>

using namespace rack;

#include <Prefabs.hpp>
#include "ModularMenuItem.hpp"

struct ModuleItem : ModularMenuItem
{
    Prefabs* module;
    std::string moduleName;
    PrefabSet prefabs;

    ModuleItem(Prefabs* module, std::string moduleName, PrefabSet prefabs);
    Menu* createChildMenu() override;
};