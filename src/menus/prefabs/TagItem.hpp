#pragma once

#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct TagItem : ModularMenuItem
{
    Prefabs* module;
    std::string tag;
    std::set<Prefab> prefabs;

    TagItem(Prefabs* module, std::string tag, std::set<Prefab> prefabs);
    Menu* createChildMenu() override;
};