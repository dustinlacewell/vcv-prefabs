#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct LibraryTagMenu : ModularMenuItem
{
    Prefabs* module;
    std::string label;
    bool favoritesOnly = false;

    LibraryTagMenu(Prefabs* module, std::string label, bool favoritesOnly = false);
};