#pragma once

#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct LibraryModuleItem : ModularMenuItem
{
    Model* model;

    LibraryModuleItem(Model* model);
};