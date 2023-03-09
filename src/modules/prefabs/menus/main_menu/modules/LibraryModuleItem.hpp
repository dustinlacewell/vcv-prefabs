#pragma once

#include <rack.hpp>

using namespace rack;

#include "ui/menus/ModularMenuItem.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"

struct LibraryModuleItem : ModularItem {
    LibraryModuleItem(std::string pluginSlug, std::string modelSlug);
    Model* model;

    LibraryModuleItem(Model* model);
    void bindModel(Model* model);
};