#include "LocalPatches.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/GroupItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"

LocalPatches::LocalPatches(MenuState* menuState) {
    this->menuState = menuState;
    buildMenu();
}

void LocalPatches::buildLocalPatchUntagged() {
    auto& localSource = menuState->state->patchesIndex.sources["local"];

    auto untagged = localSource.groups.find("untagged");
    if (untagged != localSource.groups.end()) {
        auto item = new GroupItem(menuState->state, "untagged", untagged->second);
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        addChild(item);
    }
}

void LocalPatches::buildLocalPatchTags() {
    auto& localSource = menuState->state->patchesIndex.sources["local"];
    for (auto& [groupName, tagPatches] : localSource.groups) {
        if (groupName == "untagged")
            continue;
        auto item = new GroupItem(menuState->state, groupName, tagPatches);
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        addChild(item);
    }

    buildLocalPatchUntagged();
}

void LocalPatches::buildMenu() {
    auto& localSource = menuState->state->patchesIndex.sources["local"];
    if (localSource.racks.empty()) {
        return;
    }
    buildLocalPatchTags();
}
