#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct PluginItem : ModularMenuItem
{
    State* state;
    std::string pluginName;
    ModuleMap modules;

    PluginItem(State* module, std::string pluginName, ModuleMap modules);
    Menu* createChildMenu() override;
};