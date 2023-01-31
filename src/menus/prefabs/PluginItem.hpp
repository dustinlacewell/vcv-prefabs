#pragma once

#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct PluginItem : ModularMenuItem
{
    Prefabs* module;
    std::string pluginName;
    ModuleMap modules;

    PluginItem(Prefabs* module, std::string pluginName, ModuleMap modules);
    Menu* createChildMenu() override;
};