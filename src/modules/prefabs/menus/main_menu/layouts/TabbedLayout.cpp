#include "TabbedLayout.hpp"
#include "modules/prefabs/menus/main_menu/sections/modules/FavoriteModules.hpp"
#include "modules/prefabs/menus/main_menu/sections/patches/UntuckedPatches.hpp"
#include "modules/prefabs/menus/main_menu/sections/prefabs/UntuckedPrefabs.hpp"
#include "modules/prefabs/menus/main_menu/sections/search/SearchResults.hpp"
#include "ui/TabBar.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "utils/keyboard.hpp"

TabbedLayout::TabbedLayout(MenuState* menuState) {
    this->menuState = menuState;
    this->currentTab = "Prefabs";

    std::vector<std::string> tags = {"Prefabs", "Patches", "Modules"};

    auto tabs = new TabBar(tags, [this](std::string selectedTab) { this->currentTab = selectedTab; });

    addChild(tabs);

    build();
}

void TabbedLayout::build() {
    auto prefabs = new UntuckedPrefabs(menuState);
    prefabs->visibleCallback = [this]() {
        return this->currentTab == "Prefabs";
    };

    auto patches = new UntuckedPatches(menuState);
    patches->visibleCallback = [this]() {
        return this->currentTab == "Patches";
    };

    auto modules = new FavoriteModules(menuState, !modPressed(RACK_MOD_CTRL));
    modules->visibleCallback = [this]() {
        return this->currentTab == "Modules";
    };

    std::vector<Widget*> children = {prefabs, patches, modules, new SearchResults(menuState)};

    for (auto child : children) {
        addChild(child);
    }
}
