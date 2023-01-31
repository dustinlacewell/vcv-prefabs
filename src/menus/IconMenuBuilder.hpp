#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "ModularMenuLabel.hpp"
#include "ModuleResultsBuilder.hpp"
#include "PluginItem.hpp"
#include "PrefabItem.hpp"
#include "TagItem.hpp"
#include "menus/PluginSubMenuBuilder.hpp"
#include "menus/TaggedModuleSubMenuBuilder.hpp"
#include "models/ModuleIndex.hpp"
#include "ui/ModelBox.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "utils/keyboard.hpp"

using namespace rack;

struct IconMenuBuilder
{
    Prefabs* module;
    ModuleIndex modules;
    Menu* menu;
    SearchBox* searchBox;

    IconMenuBuilder(Prefabs* module) : module(module) {}

    void createSeparator() const { menu->addChild(new MenuSeparator()); }

    template <typename T>
    T* makeDefault(T* item) const
    {
        item->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(item);
        return item;
    }

    PrefabSource& getLocalSource() const { return module->prefabs.getLocalSource(); }

    void createLocalPrefabsLabel() const
    {
        auto title = new ModularMenuLabel();
        title->text = "Local prefabs:";
        makeDefault(title);
    }

    void createLocalPrefabTags(PrefabSource& localSource) const
    {
        for (auto& [tagName, tagPrefabs] : localSource.tags) {
            if (tagName == "untagged")
                continue;
            makeDefault(new TagItem(module, tagName, tagPrefabs));
        }

        // add "untagged" tag
        auto untagged = localSource.tags.find("untagged");
        if (untagged != localSource.tags.end()) {
            makeDefault(new TagItem(module, "untagged", untagged->second));
        }
    }

    void createPrefabResults(PrefabSource& localSource) const
    {
        for (const auto& prefab : localSource.prefabs) {
            auto item = new PrefabItem(module, prefab);
            item->text = prefab.getName();
            item->visibleCallback = [this, prefab]() {
                bool nonEmpty = this->searchBox->text != "";
                bool found = prefab.getName().find(this->searchBox->text) != std::string::npos;
                return nonEmpty && found;
            };
            menu->addChild(item);
        }
    }

    void createLocalPrefabTagsByModule(const PrefabSource& localSource) const
    {
        auto pluginsItem = makeDefault(new ModularMenuItem());
        pluginsItem->text = "by module:";
        pluginsItem->childMenuCallback = [this, plugins = localSource.plugins](ModularMenuItem* item, Menu* menu) {
            for (auto [pluginName, pluginModules] : plugins) {
                auto pluginItem = new PluginItem(module, pluginName, pluginModules);
                menu->addChild(pluginItem);
            }
        };
    }

    auto createPluginPrefabsLabel()
    {
        auto title = makeDefault(new ModularMenuLabel());
        title->text = "Plugin prefabs:";
        return title;
    }

    auto createSourceMenu(const PrefabSource& source) const
    {
        std::vector<Widget*> widgets = {};

        for (auto& [tagName, tagPrefabs] : source.tags) {
            if (tagName == "untagged")
                continue;
            auto tag = new TagItem(module, tagName, tagPrefabs);
            widgets.push_back(tag);
        }

        // get untagged
        auto untagged = source.tags.find("untagged");
        if (untagged != source.tags.end()) {
            auto tag = new TagItem(module, "untagged", untagged->second);
            widgets.push_back(tag);
        }

        auto pluginsItem = new ModularMenuItem();
        pluginsItem->text = "by module:";
        pluginsItem->childMenuCallback = [this, plugins = source.plugins](ModularMenuItem* item, Menu* subMenu) {
            for (auto [pluginName, pluginModules] : plugins) {
                auto pluginItem = new PluginItem(this->module, pluginName, pluginModules);
                subMenu->addChild(pluginItem);
            }
        };
        widgets.push_back(pluginsItem);

        return widgets;
    }

    void createPluginPrefabResults()
    {
        for (const auto& source : module->prefabs.sources) {
            const auto& sourceName = source.first;
            const auto& sourcePrefabs = source.second;

            if (sourceName == "local") {
                continue;
            }

            if (sourcePrefabs.total == 0)
                continue;

            auto item = makeDefault(new ModularMenuItem());
            item->text = sourceName;
            item->childMenuCallback = [this, sourcePrefabs](ModularMenuItem* item, Menu* subMenu) {
                auto widgets = createSourceMenu(sourcePrefabs);
                for (auto widget : widgets) {
                    subMenu->addChild(widget);
                }
            };
        }
    }

    void createModuleSearchResults()
    {
        for (int i = 0; i < 128; i++) {
            auto item = buildModuleSearchResult(module, &modules, i);
            menu->addChild(item);
        }
    }

    void createModuleSearchMessage()
    {
        auto moreItem = new ModularMenuLabel();
        moreItem->visibleCallback = [this, moreItem]() {
            auto max = this->module->searchResultsQuantity.getValue();
            if (this->modules.results.size() > max) {
                moreItem->text = rack::string::f("%d more results", (int)(this->modules.results.size() - max));
                return true;
            }
            return false;
        };
        menu->addChild(moreItem);
    }

    void createModuleIndexMenu(std::string label, bool favoritesOnly)
    {
        auto all = new ModularMenuItem();
        all->text = label;
        all->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        all->childMenuCallback = [favoritesOnly](ModularMenuItem* item, Menu* indexSubMenu) {
            for (auto plugin : rack::plugin::plugins) {
                PluginSubMenuData data;
                data.plugin = plugin;
                data.modules = std::vector<Model*>();

                for (auto pluginModule : plugin->models) {
                    if (favoritesOnly && !pluginModule->isFavorite()) {
                        continue;
                    }
                    data.modules.push_back(pluginModule);
                }

                if (data.modules.size() == 0) {
                    continue;
                }

                auto pluginItem = buildPluginSubMenu(data);
                indexSubMenu->addChild(pluginItem);
            }
        };
        menu->addChild(all);
    }

    void createModuleTagIndexMenu(std::string label, bool favoritesOnly)
    {
        auto tagsItem = new ModularMenuItem();
        tagsItem->text = label;
        tagsItem->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        tagsItem->childMenuCallback = [favoritesOnly](ModularMenuItem* item, Menu* indexSubMenu) {
            auto label = new MenuLabel();
            label->text = "Tags";
            indexSubMenu->addChild(label);

            for (int tagId = 0; tagId < (int)tag::tagAliases.size(); tagId++) {
                auto tagItem = buildTaggedModuleSubMenu(tagId, favoritesOnly);

                if (tagItem == nullptr) {
                    continue;
                }

                indexSubMenu->addChild(tagItem);
            }
        };

        menu->addChild(tagsItem);
    }

    void createSearchBox()
    {
        searchBox = new SearchBox([this](std::string text) {
            this->modules.results.clear();

            if (text == "") {
                return;
            }

            this->modules.search(text);
        });
        searchBox->setExtents(Vec(250, BND_WIDGET_HEIGHT));
        menu->addChild(searchBox);
    }

    void createLocalPrefabs(PrefabSource& localSource) const
    {
        createLocalPrefabsLabel();
        createLocalPrefabTags(localSource);
        createLocalPrefabTagsByModule(localSource);
    }
    void createPluginPrefabs()
    {
        createPluginPrefabsLabel();
        createPluginPrefabResults();
    }
    void createModuleResults()
    {
        createModuleSearchResults();
        createModuleSearchMessage();
    }
    void build()
    {
        menu = createMenu();
        modules.results.clear();

        auto& localSource = module->prefabs.getLocalSource();

        createSearchBox();
        createPrefabResults(localSource);
        createLocalPrefabs(localSource);
        createPluginPrefabs();
        createModuleResults();
        createSeparator();

        bool favoritesOnly = !modPressed(RACK_MOD_CTRL);
        createModuleIndexMenu(favoritesOnly ? "Favorites:" : "Modules:", favoritesOnly);
        createModuleTagIndexMenu(favoritesOnly ? "Favorites by tag:" : "Modules by tag:", favoritesOnly);

        if (favoritesOnly) {
            auto tip = new ModularMenuLabel();
            tip->text = "Reopen with Ctrl to show all modules";
            tip->visibleCallback = [this]() {
                return searchBox->text == "";
            };
            menu->addChild(tip);
        }
    }
};