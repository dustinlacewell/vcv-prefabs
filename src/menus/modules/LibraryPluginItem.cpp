#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"

LibraryPluginItem::LibraryPluginItem(Prefabs* module, Plugin* plugin, std::vector<Model*> modules)
{
    this->module = module;
    this->plugin = plugin;
    this->modules = modules;
    this->text = plugin->name;

    this->childMenuCallback = [this](ModularMenuItem* item, Menu* pluginSubMenu) {
        for (auto moduleItem : this->buildModuleSubItems()) {
            pluginSubMenu->addChild(moduleItem);
        }
    };
}

std::vector<MenuItem*> LibraryPluginItem::buildModuleSubItems() const
{
    std::vector<MenuItem*> items;
    auto sorted = module->moduleSorter.sort(modules);

    std::transform(sorted.begin(), sorted.end(), std::back_inserter(items), [](Model* model) {
        return new LibraryModuleItem(model);
    });
    return items;
};
