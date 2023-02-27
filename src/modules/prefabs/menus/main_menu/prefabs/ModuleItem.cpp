#include "RackItem.hpp"

#include "ModuleItem.hpp"

ModuleItem::ModuleItem(State* state, std::string pluginName, std::string moduleName, RackSet racks) : racks(racks)
{
    this->state = state;
    this->pluginName = pluginName;
    this->moduleName = moduleName;
    this->text = moduleName;

    this->childMenuCallback = [racks = racks, state, pluginName, moduleName](ModularItem* self, ScrollableMenu* menu) {
        for (auto rack : racks) {
            auto item = new RackItem(state, rack);
            menu->addMenuItem(item);
        }
    };

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