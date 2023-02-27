#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/models/ModuleIndex.hpp"
#include "modules/prefabs/models/State.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"

struct LibraryResultItem : ModularItem {
    State* state;
    ModuleIndex* modules;
    int index;

    LibraryResultItem(State* state, ModuleIndex* modules, int index);
};