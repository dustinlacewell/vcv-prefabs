#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/models/State.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "ui/menus/scrolling/ScrollableItem.hpp"

struct ModuleItem : ModularItem {
    State* state;
    std::string pluginName;
    std::string moduleName;
    RackSet racks;

    ModuleItem(State* state, std::string pluginName, std::string moduleName, RackSet racks);
};