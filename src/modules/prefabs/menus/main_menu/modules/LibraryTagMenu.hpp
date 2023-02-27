#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "ui/menus/scrolling/ModularItem.hpp"

struct LibraryTagMenu : ModularItem {
    State* state;
    std::string label;
    bool favoritesOnly = false;

    LibraryTagMenu(State* state, std::string label, bool favoritesOnly = false);
};