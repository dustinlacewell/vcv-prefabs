#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"

LibraryPluginItem::LibraryPluginItem(State* state, Plugin* plugin, std::vector<Model*> modules)
{
    this->state = state;
    this->plugin = plugin;
    this->modules = modules;
    this->text = plugin->name;

    this->childMenuCallback = [this](ModularItem* item, ScrollableMenu* pluginSubMenu) {
        for (auto moduleItem : this->buildModuleSubItems()) {
            pluginSubMenu->addMenuItem(moduleItem);
        }

        pluginSubMenu->maxHeight = 300;
    };
}

std::vector<Widget*> LibraryPluginItem::buildModuleSubItems() const
{
    std::vector<Widget*> items;
    auto sorted = state->moduleSorter.sort(modules);

    std::transform(sorted.begin(), sorted.end(), std::back_inserter(items), [](Model* model) {
        return new LibraryModuleItem(model);
    });
    return items;
};
