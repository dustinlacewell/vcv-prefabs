#include "LocalPrefabs.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/GroupItem.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/PluginItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"

LocalPrefabs::LocalPrefabs(MenuState* menuState) {
    this->menuState = menuState;
    buildMenu();
}

void LocalPrefabs::buildLocalPrefabsUntagged() {
    auto& localSource = menuState->state->prefabsIndex.sources["local"];

    auto untagged = localSource.groups.find("untagged");
    if (untagged != localSource.groups.end()) {
        auto item = new GroupItem(menuState->state, "untagged", untagged->second);
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        addChild(item);
    }
}

void LocalPrefabs::buildLocalPrefabTags() {
    auto& localSource = menuState->state->prefabsIndex.sources["local"];
    for (auto& [groupName, groupPrefabs] : localSource.groups) {
        if (groupName == "untagged")
            continue;
        auto item = new GroupItem(menuState->state, groupName, groupPrefabs);
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        addChild(item);
    }

    buildLocalPrefabsUntagged();
}

void LocalPrefabs::buildLocalPrefabTagsByModule() {
    auto& localSource = menuState->state->prefabsIndex.sources["local"];
    auto pluginsItem = new ModularItem();
    pluginsItem->visibleCallback = [this]() {
        return menuState->query == "";
    };
    pluginsItem->text = "by module:";
    pluginsItem->childMenuCallback = [this, plugins = localSource.plugins](ModularItem* item, ScrollableMenu* menu) {
        for (auto [pluginName, pluginModules] : plugins) {
            auto pluginItem = new PluginItem(menuState->state, pluginName, pluginModules);
            menu->addMenuItem(pluginItem);
        }
        menu->maxHeight = 150;
    };
}

void LocalPrefabs::buildMenu() {
    buildLocalPrefabTags();
    buildLocalPrefabTagsByModule();
}
