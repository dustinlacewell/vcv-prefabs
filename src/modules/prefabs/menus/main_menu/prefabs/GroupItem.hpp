#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/models/State.hpp"
#include "modules/prefabs/sources/Rack.h"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "ui/menus/scrolling/ScrollableItem.hpp"

struct GroupItem : ModularItem {
    State* state;
    std::string group;
    std::set<Rack> racks;

    GroupItem(State* module, std::string groupName, std::set<Rack> groupRacks);
};