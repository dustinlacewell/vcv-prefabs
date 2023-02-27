#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/models/State.hpp"
#include "ui/menus/ModularMenuItem.hpp"

struct LibraryPluginItem : ModularItem {
    State* state;
    Plugin* plugin;
    std::vector<Model*> modules;

    LibraryPluginItem(State* module, Plugin* plugin, std::vector<Model*> modules);
    std::vector<Widget*> buildModuleSubItems() const;
};