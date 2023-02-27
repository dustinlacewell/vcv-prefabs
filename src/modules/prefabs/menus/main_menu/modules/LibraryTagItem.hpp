#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "ui/menus/scrolling/ModularItem.hpp"

struct PluginSubMenuData {
    Plugin* plugin;
    std::vector<Model*> modules;
};

struct LibraryTagItem : ModularItem {
    State* state;
    int tagId;
    bool favoritesOnly = false;

    std::vector<PluginSubMenuData> plugins;

    LibraryTagItem(State* state, int tagId, bool favoritesOnly = false);
};