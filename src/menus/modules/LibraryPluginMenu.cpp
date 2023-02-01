#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"
#include "LibraryPluginMenu.hpp"

LibraryPluginMenu::LibraryPluginMenu(Prefabs* module, std::string label, bool favoritesOnly)
{
    this->module = module;
    this->text = label;
    this->favoritesOnly = favoritesOnly;

    this->childMenuCallback = [module = this->module, favoritesOnly](ModularMenuItem* item, Menu* indexSubMenu) {
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

            indexSubMenu->addChild(new LibraryPluginItem(module, plugin, modules));
        }
    };
}