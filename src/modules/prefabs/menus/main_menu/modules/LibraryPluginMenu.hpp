#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "ui/menus/scrolling/ModularItem.hpp"

struct LibraryPluginMenu : ModularItem {
    State* state;
    std::string label;
    bool favoritesOnly = false;

    LibraryPluginMenu(State* state, std::string label, bool favoritesOnly = false);
};