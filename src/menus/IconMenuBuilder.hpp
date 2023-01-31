#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "ModularMenuLabel.hpp"
#include "menus/modules/LibraryPluginItem.hpp"
#include "menus/modules/LibraryPluginMenu.hpp"
#include "menus/modules/LibraryResultItem.hpp"
#include "menus/modules/LibraryTagItem.hpp"
#include "menus/modules/LibraryTagMenu.hpp"
#include "menus/prefabs/PluginItem.hpp"
#include "menus/prefabs/PrefabItem.hpp"
#include "menus/prefabs/TagItem.hpp"
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

    IconMenuBuilder(Prefabs* module) : module(module) { modules = ModuleIndex(module); }

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

    void createLocalPrefabTags() const
    {
        auto& localSource = module->prefabs.getLocalSource();
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

    void createPrefabResults() const
    {
        auto label = new ModularMenuLabel();
        label->text = "Prefabs:";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        auto& localSource = module->prefabs.getLocalSource();
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

    void createLocalPrefabTagsByModule() const
    {
        auto& localSource = module->prefabs.getLocalSource();
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
            auto item = new LibraryResultItem(module, &modules, i);
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

    void createModuleIndexMenu()
    {
        bool favoritesOnly = !modPressed(RACK_MOD_CTRL);
        auto label = favoritesOnly ? "Favorites:" : "Modules:";
        auto pluginMenu = new LibraryPluginMenu(module, label, favoritesOnly);
        pluginMenu->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(pluginMenu);
    }

    void createModuleTagIndexMenu()
    {
        bool favoritesOnly = !modPressed(RACK_MOD_CTRL);
        auto label = favoritesOnly ? "Favorites by tag:" : "Modules by tag:";
        auto tagIndexMenu = new LibraryTagMenu(module, label, favoritesOnly);
        tagIndexMenu->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(tagIndexMenu);
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

    void createLocalPrefabs() const
    {
        createLocalPrefabsLabel();
        createLocalPrefabTags();
        createLocalPrefabTagsByModule();
    }
    void createPluginPrefabs()
    {
        createPluginPrefabsLabel();
        createPluginPrefabResults();
    }
    void createModuleResults()
    {
        auto label = new ModularMenuLabel();
        label->text = "Modules:";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        createModuleSearchResults();
        createModuleSearchMessage();
    }

    void createLibrary()
    {
        createModuleIndexMenu();
        createModuleTagIndexMenu();

        if (!modPressed(RACK_MOD_CTRL)) {
            auto tip = new ModularMenuLabel();
            tip->text = "Reopen with Ctrl to show all modules";
            tip->visibleCallback = [this]() {
                return searchBox->text == "";
            };
            menu->addChild(tip);
        }
    }

    void build()
    {
        menu = createMenu();
        modules.results.clear();

        createSearchBox();
        createPrefabResults();
        createLocalPrefabs();
        createPluginPrefabs();
        createSeparator();
        createModuleResults();
        createLibrary();
    }
};