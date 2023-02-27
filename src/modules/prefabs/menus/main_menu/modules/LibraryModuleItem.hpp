#pragma once

#include <rack.hpp>

using namespace rack;

#include "ui/menus/ModularMenuItem.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"

struct LibraryModuleItem : ModularItem {
    Model* model;

    LibraryModuleItem(Model* model);
};