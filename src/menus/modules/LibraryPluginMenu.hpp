#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct LibraryPluginMenu : ModularMenuItem
{
    Prefabs* module;
    std::string label;
    bool favoritesOnly = false;

    LibraryPluginMenu(Prefabs* module, std::string label, bool favoritesOnly = false);
};