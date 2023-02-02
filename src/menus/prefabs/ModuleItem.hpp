#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct ModuleItem : ModularMenuItem
{
    State* state;
    std::string moduleName;
    PrefabSet prefabs;

    ModuleItem(State* state, std::string moduleName, PrefabSet prefabs);
    Menu* createChildMenu() override;
};