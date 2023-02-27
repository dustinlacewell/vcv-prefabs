#pragma once

#include <rack.hpp>
#include "modules/prefabs/menus/main_menu/MenuState.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "ui/menus/scrolling/ScrollableMenu.hpp"

using namespace rack;

struct LocalPrefabs : VerticalGroup {

    MenuState* menuState;

    LocalPrefabs(MenuState* menuState);

    void buildLocalPrefabsUntagged();
    void buildLocalPrefabTags();
    void buildLocalPrefabTagsByModule();

    void buildMenu();
    void buildResultsSubmenu();
};