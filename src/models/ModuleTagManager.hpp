#pragma once

#include <rack.hpp>
#include "menus/ModularMenuItem.hpp"

using namespace rack;

struct ModuleTagManager
{
    std::set<int> hiddenTags;

    ModuleTagManager();
    void toggleTag(int tagId);
    MenuItem* createToggleMenu();
    json_t* toJson();
    void fromJson(json_t* rootJ);
};