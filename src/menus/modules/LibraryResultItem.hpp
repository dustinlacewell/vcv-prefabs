#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/ModuleIndex.hpp"
#include "models/State.hpp"

struct LibraryResultItem : ModularMenuItem
{
    State* state;
    ModuleIndex* modules;
    int index;

    LibraryResultItem(State* state, ModuleIndex* modules, int index);
};