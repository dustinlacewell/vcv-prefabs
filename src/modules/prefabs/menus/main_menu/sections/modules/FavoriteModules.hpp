#pragma once

#include "modules/prefabs/menus/main_menu/MenuState.hpp"
#include "ui/VerticalGroup.hpp"

struct FavoriteModules : VerticalGroup {

    MenuState* menuState;
    bool favoritesOnly;

    FavoriteModules(MenuState* menuState, bool favoritesOnly);

    void buildMenu();
    void createModuleIndexMenu();
    void createModuleTagIndexMenu();
};