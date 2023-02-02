#include <rack.hpp>

#include "ModuleItem.hpp"
#include "PluginItem.hpp"

PluginItem::PluginItem(State* state, std::string pluginName, ModuleMap modules)
{
    this->state = state;
    this->pluginName = pluginName;
    this->rightText = RIGHT_ARROW;
    this->text = pluginName;
    this->modules = modules;
}

Menu* PluginItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto [moduleName, prefabSet] : modules) {
        auto item = new ModuleItem(state, moduleName, prefabSet);
        menu->addChild(item);
    }
    return menu;
}