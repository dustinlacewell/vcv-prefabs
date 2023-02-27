#include "PluginPrefabs.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/GroupItem.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/PluginItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"

PluginPrefabs::PluginPrefabs(MenuState* menuState)
{
    this->menuState = menuState;
    buildMenu();
}

void PluginPrefabs::buildLabel()
{
    auto title = new ModularItem();
    title->visibleCallback = [this]() {
        return menuState->query == "";
    };
    title->text = "Plugin prefabs:";
    title->disabled = true;
    addChild(title);
}
void PluginPrefabs::buildLocalPrefabTagsByModule()
{
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

auto PluginPrefabs::buildPrefabSourceMenu(const SourceIndex source)
{
    std::vector<Widget*> widgets = {};

    // tagged
    for (auto& [groupName, groupPrefabs] : source.groups) {
        if (groupName == "untagged")
            continue;
        auto tag = new GroupItem(menuState->state, groupName, groupPrefabs);
        widgets.push_back(tag);
    }

    // untagged
    auto untagged = source.groups.find("untagged");
    if (untagged != source.groups.end()) {
        auto tag = new GroupItem(menuState->state, "untagged", untagged->second);
        widgets.push_back(tag);
    }

    // by module
    auto pluginsItem = new ModularItem();
    pluginsItem->text = "by module:";
    pluginsItem->childMenuCallback = [this, plugins = source.plugins](ModularItem* item, ScrollableMenu* subMenu) {
        for (auto [pluginName, pluginModules] : plugins) {
            auto pluginItem = new PluginItem(menuState->state, pluginName, pluginModules);
            subMenu->addMenuItem(pluginItem);
        }
    };
    widgets.push_back(pluginsItem);

    return widgets;
}

void PluginPrefabs::buildItems()
{
    for (const auto& source : menuState->state->prefabsIndex.sources) {
        const auto& sourceName = source.first;
        const auto& sourceIndex = source.second;

        if (sourceName == "local") {
            continue;
        }

        if (sourceIndex.racks.size() == 0)
            continue;

        auto item = new ModularItem();
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        item->text = sourceName;
        item->childMenuCallback = [this, sourceIndex](ModularItem* item, ScrollableMenu* subMenu) {
            auto widgets = buildPrefabSourceMenu(sourceIndex);
            for (auto widget : widgets) {
                subMenu->addMenuItem(widget);
            }
        };
        addChild(item);
    }
}

void PluginPrefabs::buildMenu()
{
    buildLabel();
    buildItems();
}
