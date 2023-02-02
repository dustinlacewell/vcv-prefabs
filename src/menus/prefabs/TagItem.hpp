#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct TagItem : ModularMenuItem
{
    State* state;
    std::string tag;
    std::set<Prefab> prefabs;

    TagItem(State* module, std::string tag, std::set<Prefab> prefabs);
    Menu* createChildMenu() override;
};