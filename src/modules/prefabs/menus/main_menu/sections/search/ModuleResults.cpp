#include "ModuleResults.hpp"
#include "modules/prefabs/menus/main_menu/modules/LibraryResultItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"

ModuleResults::ModuleResults(MenuState* menuState) {
    this->menuState = menuState;
    buildMenu();
}

void ModuleResults::buildLabel() {
    auto sep = createSeparator();
    sep->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep);

    auto label = new ModularMenuLabel();
    label->text = "Modules";
    label->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(label);

    auto sep2 = createSeparator();
    sep2->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep2);
}

void ModuleResults::buildModuleSearchMessage() {
    auto moreItem = new ModularMenuLabel();
    moreItem->visibleCallback = [this, moreItem]() {
        auto max = menuState->state->searchResultsQuantity.getValue();
        if (menuState->moduleIndex.results.size() > max) {
            moreItem->text = rack::string::f("%d more results", (int)(menuState->moduleIndex.results.size() - max));
            return true;
        }
        return false;
    };
    addChild(moreItem);
}

void ModuleResults::buildResultsSubmenu() {
    auto subMenu = new ScrollableMenu();
    subMenu->maxHeight = 200;

    for (int i = 0; i < 128; i++) {
        auto item = new LibraryResultItem(menuState->state, &menuState->moduleIndex, i);
        subMenu->addMenuItem(item);
    }

    addChild(subMenu);
}

void ModuleResults::buildMenu() {
    buildLabel();
    buildResultsSubmenu();
    buildModuleSearchMessage();
}
