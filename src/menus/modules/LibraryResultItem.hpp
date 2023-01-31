#pragma once

#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"
#include "models/ModuleIndex.hpp"

struct LibraryResultItem : ModularMenuItem
{
    Prefabs* module;
    ModuleIndex* modules;
    int index;

    LibraryResultItem(Prefabs* module, ModuleIndex* modules, int index);
};