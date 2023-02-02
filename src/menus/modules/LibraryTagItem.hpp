#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct PluginSubMenuData
{
    Plugin* plugin;
    std::vector<Model*> modules;
};

struct LibraryTagItem : ModularMenuItem
{
    State* state;
    int tagId;
    bool favoritesOnly = false;

    std::vector<PluginSubMenuData> plugins;

    LibraryTagItem(State* state, int tagId, bool favoritesOnly = false);
};