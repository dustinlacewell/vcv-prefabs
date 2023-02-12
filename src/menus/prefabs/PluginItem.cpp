#include <rack.hpp>

#include "ModuleItem.hpp"
#include "PluginItem.hpp"

PluginItem::PluginItem(State* state, std::string pluginName, RackGroups modules)
{
    this->state = state;
    this->pluginName = pluginName;
    this->rightText = RIGHT_ARROW;
    this->text = pluginName;
    this->modules = modules;

    bool hasPlugin = false;
    for (auto plugin : plugin::plugins) {
        if (plugin->slug == pluginName) {
            hasPlugin = true;
            break;
        }
    }

    if (!hasPlugin) {
        this->disabled = true;
    }
}

Menu* PluginItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto [moduleName, rackSet] : modules) {
        auto item = new ModuleItem(state, this->pluginName, moduleName, rackSet);
        menu->addChild(item);
    }
    return menu;
}