#include <rack.hpp>

#include "ModuleItem.hpp"
#include "PluginItem.hpp"

PluginItem::PluginItem(State* state, std::string pluginName, RackGroups modules)
{
    this->state = state;
    this->pluginName = pluginName;
    this->text = pluginName;
    this->modules = modules;

    this->childMenuCallback = [modules = modules, state, pluginName](ModularItem* self, ScrollableMenu* menu) {
        for (auto [moduleName, rackSet] : modules) {
            auto item = new ModuleItem(state, pluginName, moduleName, rackSet);
            menu->addMenuItem(item);
        }

        menu->maxHeight = 300;
    };

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