#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct ModuleItem : ModularMenuItem
{
    State* state;
    std::string moduleName;
    RackSet racks;

    ModuleItem(State* state, std::string moduleName, RackSet racks);
    Menu* createChildMenu() override;
};