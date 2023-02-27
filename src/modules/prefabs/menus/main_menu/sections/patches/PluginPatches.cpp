#include "PluginPatches.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/GroupItem.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/PluginItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"

PluginPatches::PluginPatches(MenuState* menuState)
{
    this->menuState = menuState;
    buildMenu();
}

void PluginPatches::buildLabel()
{
    auto title = new ModularItem();
    title->visibleCallback = [this]() {
        return menuState->query == "";
    };
    title->text = "Plugin patches:";
    title->disabled = true;
    addChild(title);
}

auto PluginPatches::buildPatchSourceMenu(const SourceIndex source)
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

void PluginPatches::buildItems()
{
    for (const auto& source : menuState->state->patchesIndex.sources) {
        const auto& sourceName = source.first;
        const auto& sourceIndex = source.second;

        if (sourceName == "local") {
            continue;
        }

        if (sourceIndex.racks.empty())
            continue;

        auto item = new ModularItem();
        item->visibleCallback = [this]() {
            return menuState->query == "";
        };
        item->text = sourceName;
        item->childMenuCallback = [this, sourceIndex](ModularItem* item, ScrollableMenu* subMenu) {
            auto widgets = buildPatchSourceMenu(sourceIndex);
            for (auto widget : widgets) {
                subMenu->addMenuItem(widget);
            }

            subMenu->maxHeight = 300;
        };
    }
}

void PluginPatches::buildMenu()
{
    auto sources = menuState->state->store.patchSources;
    auto total = 0;
    for (auto source : sources) {
        if (source->getSlug() == "local")
            continue;
        total += source->getTotal();
    }
    if (total == 0)
        return;

    buildLabel();
    buildItems();
}
