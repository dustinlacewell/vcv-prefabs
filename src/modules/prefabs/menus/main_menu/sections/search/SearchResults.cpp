#include "SearchResults.hpp"
#include "ModuleResults.hpp"
#include "PatchResults.hpp"
#include "PrefabResults.hpp"
#include "ui/menus/ModularMenuLabel.hpp"

SearchResults::SearchResults(MenuState* menuState) {
    this->menuState = menuState;
    buildMenu();
}

void SearchResults::buildMenu() {
    auto prefabResults = new PrefabResults(menuState);
    addChild(prefabResults);

    auto patchResults = new PatchResults(menuState);
    addChild(patchResults);

    auto moduleResults = new ModuleResults(menuState);
    addChild(moduleResults);
}
