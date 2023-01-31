#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct PluginSubMenuData
{
    Plugin* plugin;
    std::vector<Model*> modules;
};

struct LibraryTagItem : ModularMenuItem
{
    Prefabs* module;
    int tagId;
    bool favoritesOnly = false;

    std::vector<PluginSubMenuData> plugins;

    LibraryTagItem(Prefabs* module, int tagId, bool favoritesOnly = false);
};