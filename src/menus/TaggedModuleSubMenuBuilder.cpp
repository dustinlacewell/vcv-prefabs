#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "menus/PluginSubMenuBuilder.hpp"

using namespace rack;

#include "TaggedModuleSubMenuBuilder.hpp"

MenuItem* buildTaggedModuleSubMenu(int tagId, bool favoritesOnly)
{
    auto tag = tag::getTag(tagId);
    auto pluginsWithTag = std::vector<PluginSubMenuData>();

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
            pluginsWithTag.push_back(data);
        }
    }

    if (pluginsWithTag.size() == 0) {
        return nullptr;
    }

    auto tagItem = new ModularMenuItem();
    tagItem->text = tag;
    tagItem->childMenuCallback = [pluginsWithTag](ModularMenuItem* item, Menu* tagSubMenu) {
        for (auto data : pluginsWithTag) {
            auto subMenu = buildPluginSubMenu(data);
            tagSubMenu->addChild(subMenu);
        }
    };

    return tagItem;
}