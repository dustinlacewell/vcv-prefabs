#pragma once

#include "modules/prefabs/models/ModuleIndex.hpp"
#include "modules/prefabs/models/RackSearcher.hpp"
#include "modules/prefabs/models/State.hpp"
#include "ui/menus/scrolling/ScrollableMenu.hpp"

struct MenuState {
    State* state;
    ModuleIndex moduleIndex;
    ScrollableMenu* rootMenu;
    std::string query;
    RackSearcher patchResults;
};