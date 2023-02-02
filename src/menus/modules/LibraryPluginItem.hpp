#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct LibraryPluginItem : ModularMenuItem
{
    State* state;
    Plugin* plugin;
    std::vector<Model*> modules;

    LibraryPluginItem(State* module, Plugin* plugin, std::vector<Model*> modules);
    std::vector<MenuItem*> buildModuleSubItems() const;
};