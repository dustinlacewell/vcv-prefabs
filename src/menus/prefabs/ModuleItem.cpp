#include "RackItem.hpp"

#include "ModuleItem.hpp"

ModuleItem::ModuleItem(State* state, std::string pluginName, std::string moduleName, RackSet racks) : racks(racks)
{
    this->state = state;
    this->pluginName = pluginName;
    this->moduleName = moduleName;
    this->text = moduleName;
    this->rightText = RIGHT_ARROW;

    bool hasModule = false;
    for (auto plugin : plugin::plugins) {
        if (plugin->slug == pluginName) {
            for (auto module : plugin->models) {
                if (module->slug == moduleName) {
                    hasModule = true;
                    break;
                }
            }
        }
    }

    if (!hasModule) {
        this->disabled = true;
    }
}

Menu* ModuleItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto rack : racks) {
        auto item = new RackItem(state, rack);
        menu->addChild(item);
    }
    return menu;
}