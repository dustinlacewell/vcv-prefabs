#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"
#include "LibraryTagItem.hpp"

LibraryTagItem::LibraryTagItem(State* state, int tagId, bool favoritesOnly)
{
    this->state = state;
    this->tagId = tagId;
    this->text = tag::getTag(tagId);
    this->favoritesOnly = favoritesOnly;

    this->plugins = std::vector<PluginSubMenuData>();

    for (auto& plugin : rack::plugin::plugins) {
        auto data = PluginSubMenuData();
        data.plugin = plugin;
        data.modules = std::vector<Model*>();

        for (auto pluginModule : plugin->models) {
            if (std::find(pluginModule->tagIds.begin(), pluginModule->tagIds.end(), tagId) !=
                pluginModule->tagIds.end()) {
                if (favoritesOnly && !pluginModule->isFavorite())
                    continue;
                data.modules.push_back(pluginModule);
            }
        }
        if (data.modules.size() > 0) {
            this->plugins.push_back(data);
        }
    }

    this->visibleCallback = [this]() {
        return this->plugins.size() > 0;
    };

    this->childMenuCallback = [this](ModularItem* item, ScrollableMenu* tagSubMenu) {
        for (auto data : this->plugins) {
            auto subMenu = new LibraryPluginItem(this->state, data.plugin, data.modules);
            tagSubMenu->addMenuItem(subMenu);
        }
    };
}
