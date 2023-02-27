#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/index/typedefs.hpp"
#include "modules/prefabs/models/State.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"

struct PluginItem : ModularItem {
    State* state;
    std::string pluginName;
    RackGroups modules;

    PluginItem(State* module, std::string pluginName, RackGroups modules);
};