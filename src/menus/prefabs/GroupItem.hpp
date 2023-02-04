#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct GroupItem : ModularMenuItem
{
    State* state;
    std::string group;
    std::set<Rack> racks;

    GroupItem(State* module, std::string groupName, std::set<Rack> groupRacks);
    Menu* createChildMenu() override;
};