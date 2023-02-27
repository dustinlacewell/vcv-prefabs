#include "FavoriteModules.hpp"
#include "modules/prefabs/menus/main_menu/modules/LibraryPluginMenu.hpp"
#include "modules/prefabs/menus/main_menu/modules/LibraryTagMenu.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "utils/keyboard.hpp"

FavoriteModules::FavoriteModules(MenuState* menuState, bool favoritesOnly) {
    this->menuState = menuState;
    this->favoritesOnly = favoritesOnly;
    buildMenu();
}

void FavoriteModules::createModuleIndexMenu() {
    auto label = "by plugin:";
    auto pluginMenu = new LibraryPluginMenu(menuState->state, label, favoritesOnly);
    pluginMenu->visibleCallback = [this]() {
        return menuState->query == "";
    };
    addChild(pluginMenu);
}

void FavoriteModules::createModuleTagIndexMenu() {
    auto label = "by tag:";
    auto tagIndexMenu = new LibraryTagMenu(menuState->state, label, favoritesOnly);
    tagIndexMenu->visibleCallback = [this]() {
        return menuState->query == "";
    };
    addChild(tagIndexMenu);
}

void FavoriteModules::buildMenu() {
    createModuleIndexMenu();
    createModuleTagIndexMenu();

    if (!modPressed(RACK_MOD_CTRL)) {
        auto tip = new ModularItem();
        tip->text = "Reopen with Ctrl to show all modules";
        tip->disabled = true;
        tip->visibleCallback = [this]() {
            return menuState->query == "";
        };
        addChild(tip);
    }
}
