#pragma once

#include <rack.hpp>
#include "modules/prefabs/menus/main_menu/MenuState.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "ui/menus/scrolling/ScrollableMenu.hpp"

using namespace rack;

struct StoragePatches : VerticalGroup {

    MenuState* menuState;

    StoragePatches(MenuState* menuState);

    void buildLabel();
    void buildItems();
    void buildMenu();
    auto buildStoragePatchSourceMenu(const UserIndex user);
};