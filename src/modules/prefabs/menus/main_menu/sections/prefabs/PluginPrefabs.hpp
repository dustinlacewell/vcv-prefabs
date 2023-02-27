#pragma once

#include <rack.hpp>
#include "modules/prefabs/menus/main_menu/MenuState.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "ui/menus/scrolling/ScrollableMenu.hpp"

using namespace rack;

struct PluginPrefabs : VerticalGroup {

    MenuState* menuState;

    PluginPrefabs(MenuState* menuState);

    void buildLocalPrefabsUntagged();
    void buildLocalPrefabTags();
    void buildLocalPrefabTagsByModule();

    void buildMenu();
    void buildLabel();
    void buildResultsSubmenu();
    auto buildPrefabSourceMenu(const SourceIndex source);
    void buildItems();
};