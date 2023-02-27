#include "StoragePatches.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/GroupItem.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/PluginItem.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/RackItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"
#include "utils/keyboard.hpp"

StoragePatches::StoragePatches(MenuState* menuState)
{
    this->menuState = menuState;
    buildMenu();
}

void StoragePatches::buildLabel()
{
    auto title = new ModularItem();
    title->visibleCallback = [this]() {
        return menuState->query == "";
    };
    title->text = "Patch Storage:";
    title->disabled = true;
    addChild(title);
}

auto StoragePatches::buildStoragePatchSourceMenu(const UserIndex user)
{
    if (user.racks.empty()) {
        return std::vector<Widget*>{};
    }

    std::vector<Widget*> widgets = {};

    // tagged
    for (auto& rack : user.sortedRacks) {
        if (!rack.isValid && !modPressed(RACK_MOD_CTRL))
            continue;

        auto item = new RackItem(menuState->state, rack);
        item->rightText = rack.isValid ? "" : "!";
        widgets.push_back(item);
    }

    // by module
    auto pluginsItem = new ModularItem();
    pluginsItem->text = "by module:";
    pluginsItem->childMenuCallback = [this, plugins = user.plugins](ModularItem* item, ScrollableMenu* subMenu) {
        for (auto [pluginName, pluginModules] : plugins) {
            auto pluginItem = new PluginItem(menuState->state, pluginName, pluginModules);
            if (!pluginItem->disabled) {
                subMenu->addMenuItem(pluginItem);
            }
        }

        subMenu->maxHeight = 300;
    };
    widgets.push_back(pluginsItem);

    return widgets;
}

void StoragePatches::buildItems()
{
    for (const auto& source : menuState->state->storageIndex.users) {
        const auto& sourceName = source.first;
        const auto& sourceIndex = source.second;

        if (sourceIndex.racks.empty())
            continue;

        auto item = new ModularItem();
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        item->text = sourceName;
        item->childMenuCallback = [this, sourceIndex](ModularItem* item, ScrollableMenu* subMenu) {
            auto widgets = buildStoragePatchSourceMenu(sourceIndex);
            for (auto widget : widgets) {
                subMenu->addMenuItem(widget);
            }

            subMenu->maxHeight = 300;
        };
        addChild(item);
    }
}

void StoragePatches::buildMenu()
{
    auto sources = menuState->state->store.userStorageSources;
    auto total = 0;
    for (auto source : sources) {
        total += source->getTotal();
    }
    if (total == 0)
        return;

    buildLabel();
    buildItems();
}
