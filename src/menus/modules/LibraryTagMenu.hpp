#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct LibraryTagMenu : ModularMenuItem
{
    State* state;
    std::string label;
    bool favoritesOnly = false;

    LibraryTagMenu(State* state, std::string label, bool favoritesOnly = false);
};