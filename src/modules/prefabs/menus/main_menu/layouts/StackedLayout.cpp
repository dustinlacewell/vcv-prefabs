#include "StackedLayout.hpp"
#include "modules/prefabs/menus/main_menu/SectionHeader.hpp"
#include "modules/prefabs/menus/main_menu/sections/modules/FavoriteModules.hpp"
#include "modules/prefabs/menus/main_menu/sections/patches/UntuckedPatches.hpp"
#include "modules/prefabs/menus/main_menu/sections/prefabs/UntuckedPrefabs.hpp"
#include "modules/prefabs/menus/main_menu/sections/search/SearchResults.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "utils/keyboard.hpp"

StackedLayout::StackedLayout(MenuState* menuState) {
    this->menuState = menuState;
    build();
}

void StackedLayout::build() {
    std::vector<Widget*> children = {new SectionHeader("Prefabs", true),
        new UntuckedPrefabs(menuState),
        new SectionHeader("Patches", true),
        new UntuckedPatches(menuState),
        new SectionHeader("Modules", true),
        new FavoriteModules(menuState, !modPressed(RACK_MOD_CTRL)),
        new SearchResults(menuState)};

    for (auto child : children) {
        addChild(child);
    }
}
