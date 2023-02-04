#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct PluginItem : ModularMenuItem
{
    State* state;
    std::string pluginName;
    RackGroups modules;

    PluginItem(State* module, std::string pluginName, RackGroups modules);
    Menu* createChildMenu() override;
};