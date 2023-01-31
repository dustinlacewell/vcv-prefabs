#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct LibraryPluginItem : ModularMenuItem
{
    Prefabs* module;
    Plugin* plugin;
    std::vector<Model*> modules;

    LibraryPluginItem(Prefabs* module, Plugin* plugin, std::vector<Model*> modules);
    std::vector<MenuItem*> buildModuleSubItems() const;
};