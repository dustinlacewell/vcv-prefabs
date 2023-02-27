#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"
#include "LibraryPluginMenu.hpp"

LibraryPluginMenu::LibraryPluginMenu(State* state, std::string label, bool favoritesOnly)
{
    this->state = state;
    this->text = label;
    this->favoritesOnly = favoritesOnly;

    this->childMenuCallback = [module = this->state, favoritesOnly](ModularItem* item, ScrollableMenu* indexSubMenu) {
        for (auto plugin : rack::plugin::plugins) {
            auto modules = std::vector<Model*>();

            for (auto pluginModule : plugin->models) {
                if (favoritesOnly && !pluginModule->isFavorite()) {
                    continue;
                }
                modules.push_back(pluginModule);
            }

            if (modules.size() == 0) {
                continue;
            }

            indexSubMenu->addMenuItem(new LibraryPluginItem(module, plugin, modules));
        }

        indexSubMenu->maxHeight = 300;
    };
}