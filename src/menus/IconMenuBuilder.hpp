#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "ModularMenuLabel.hpp"
#include "ModularMenuSeparator.hpp"
#include "menus/modules/LibraryPluginItem.hpp"
#include "menus/modules/LibraryPluginMenu.hpp"
#include "menus/modules/LibraryResultItem.hpp"
#include "menus/modules/LibraryTagItem.hpp"
#include "menus/modules/LibraryTagMenu.hpp"
#include "menus/prefabs/PluginItem.hpp"
#include "menus/prefabs/PrefabItem.hpp"
#include "menus/prefabs/TagItem.hpp"
#include "models/ModuleIndex.hpp"
#include "models/State.hpp"
#include "ui/ModelBox.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "utils/keyboard.hpp"

using namespace rack;

struct IconMenuBuilder
{
    State* state;
    ModuleIndex modules;
    Menu* menu;
    SearchBox* searchBox;

    IconMenuBuilder(State* state) : state(state) { modules = ModuleIndex(state); }

    ModularMenuSeparator* createSeparator() const
    {
        auto w = new ModularMenuSeparator();
        menu->addChild(w);
        return w;
    }

    template <typename T>
    T* makeDefault(T* item) const
    {
        item->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(item);
        return item;
    }

    PrefabSource& getLocalPrefabSource() const { return state->prefabs.getLocalSource(); }
    PrefabSource& getLocalPatchSource() const { return state->patches.getLocalSource(); }

    /**
     * Search
     */

    void createSearchBox()
    {
        searchBox = new SearchBox([this](std::string text) {
            modules.results.clear();

            if (text == "") {
                return;
            }

            modules.search(text);
        });
        searchBox->setExtents(Vec(250, BND_WIDGET_HEIGHT));
        menu->addChild(searchBox);
    }

    void createPrefabResults() const
    {
        auto label = new ModularMenuLabel();
        label->text = "Prefabs:";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        auto& localSource = state->prefabs.getLocalSource();
        for (const auto& prefab : localSource.prefabs) {
            auto item = new PrefabItem(state, prefab);
            item->text = prefab.getName();
            item->visibleCallback = [this, prefab]() {
                bool nonEmpty = this->searchBox->text != "";
                bool found = prefab.getName().find(this->searchBox->text) != std::string::npos;
                return nonEmpty && found;
            };
            menu->addChild(item);
        }
    }

    void createModuleSearchResults()
    {
        for (int i = 0; i < 128; i++) {
            auto item = new LibraryResultItem(state, &modules, i);
            menu->addChild(item);
        }
    }

    void createModuleSearchMessage()
    {
        auto moreItem = new ModularMenuLabel();
        moreItem->visibleCallback = [this, moreItem]() {
            auto max = this->state->searchResultsQuantity.getValue();
            if (this->modules.results.size() > max) {
                moreItem->text = rack::string::f("%d more results", (int)(this->modules.results.size() - max));
                return true;
            }
            return false;
        };
        menu->addChild(moreItem);
    }

    /**
     * Prefabs
     */

    // local prefabs

    void createLocalPrefabsLabel() const
    {
        auto title = new ModularMenuLabel();
        title->text = "Local prefabs:";
        makeDefault(title);
    }

    void createLocalPrefabUntagged() const
    {
        auto& localSource = state->prefabs.getLocalSource();

        auto untagged = localSource.tags.find("untagged");
        if (untagged != localSource.tags.end()) {
            makeDefault(new TagItem(state, "untagged", untagged->second));
        }
    }

    void createLocalPrefabTags() const
    {
        auto& localSource = state->prefabs.getLocalSource();
        for (auto& [tagName, tagPrefabs] : localSource.tags) {
            if (tagName == "untagged")
                continue;
            makeDefault(new TagItem(state, tagName, tagPrefabs));
        }

        createLocalPrefabUntagged();
    }

    void createLocalPrefabTagsByModule() const
    {
        auto& localSource = state->prefabs.getLocalSource();
        auto pluginsItem = makeDefault(new ModularMenuItem());
        pluginsItem->text = "by module:";
        pluginsItem->childMenuCallback = [this, plugins = localSource.plugins](ModularMenuItem* item, Menu* menu) {
            for (auto [pluginName, pluginModules] : plugins) {
                auto pluginItem = new PluginItem(state, pluginName, pluginModules);
                menu->addChild(pluginItem);
            }
        };
    }

    void createLocalPrefabs() const
    {
        createLocalPrefabsLabel();
        createLocalPrefabTags();
        createLocalPrefabTagsByModule();
    }

    // plugin prefabs

    auto createPluginPrefabsLabel() const
    {
        auto title = makeDefault(new ModularMenuLabel());
        title->text = "Plugin prefabs:";
        return title;
    }

    auto createPrefabSourceMenu(const PrefabSource& source) const
    {
        std::vector<Widget*> widgets = {};

        // tagged
        for (auto& [tagName, tagPrefabs] : source.tags) {
            if (tagName == "untagged")
                continue;
            auto tag = new TagItem(state, tagName, tagPrefabs);
            widgets.push_back(tag);
        }

        // untagged
        auto untagged = source.tags.find("untagged");
        if (untagged != source.tags.end()) {
            auto tag = new TagItem(state, "untagged", untagged->second);
            widgets.push_back(tag);
        }

        // by module
        auto pluginsItem = new ModularMenuItem();
        pluginsItem->text = "by module:";
        pluginsItem->childMenuCallback = [this, plugins = source.plugins](ModularMenuItem* item, Menu* subMenu) {
            for (auto [pluginName, pluginModules] : plugins) {
                auto pluginItem = new PluginItem(this->state, pluginName, pluginModules);
                subMenu->addChild(pluginItem);
            }
        };
        widgets.push_back(pluginsItem);

        return widgets;
    }

    void createPluginPrefabItems() const
    {
        for (const auto& source : state->prefabs.sources) {
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
                auto widgets = createPrefabSourceMenu(sourcePrefabs);
                for (auto widget : widgets) {
                    subMenu->addChild(widget);
                }
            };
        }
    }

    void createPluginPrefabs() const
    {
        createPluginPrefabsLabel();
        createPluginPrefabItems();
    }

    void createPrefabs() const
    {
        createLocalPrefabs();
        createPluginPrefabs();
        createPrefabResults();
    }

    /**
     * Patches
     */

    // local patches

    void createLocalPatchesLabel() const
    {
        auto title = new ModularMenuLabel();
        title->text = "Local patches:";
        makeDefault(title);
    }

    void createLocalPatchUntagged() const
    {
        auto& localSource = state->patches.getLocalSource();

        auto untagged = localSource.tags.find("untagged");
        if (untagged != localSource.tags.end()) {
            makeDefault(new TagItem(state, "untagged", untagged->second));
        }
    }

    void createLocalPatchTags() const
    {
        auto& localSource = state->patches.getLocalSource();
        for (auto& [tagName, tagPatches] : localSource.tags) {
            if (tagName == "untagged")
                continue;
            makeDefault(new TagItem(state, tagName, tagPatches));
        }

        createLocalPatchUntagged();
    }

    void createLocalPatches() const
    {
        createLocalPatchesLabel();
        createLocalPatchTags();
    }

    // plugin patches

    auto createPluginPatchLabel() const
    {
        auto title = makeDefault(new ModularMenuLabel());
        title->text = "Plugin patches:";
        return title;
    }

    auto createPatchSourceMenu(const PatchSource& source) const
    {
        if (source.total == 0) {
            return std::vector<Widget*>{};
        }

        std::vector<Widget*> widgets = {};

        // tagged
        for (auto& [tagName, tagPatches] : source.tags) {
            if (tagName == "untagged")
                continue;
            auto tag = new TagItem(state, tagName, tagPatches);
            widgets.push_back(tag);
        }

        // untagged
        auto untagged = source.tags.find("untagged");
        if (untagged != source.tags.end()) {
            auto tag = new TagItem(state, "untagged", untagged->second);
            widgets.push_back(tag);
        }

        // by module
        auto pluginsItem = new ModularMenuItem();
        pluginsItem->text = "by module:";
        pluginsItem->childMenuCallback = [this, plugins = source.plugins](ModularMenuItem* item, Menu* subMenu) {
            for (auto [pluginName, pluginModules] : plugins) {
                auto pluginItem = new PluginItem(this->state, pluginName, pluginModules);
                subMenu->addChild(pluginItem);
            }
        };
        widgets.push_back(pluginsItem);

        return widgets;
    }

    void createPluginPatchItems() const
    {
        for (const auto& source : state->patches.sources) {
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
                auto widgets = createPrefabSourceMenu(sourcePrefabs);
                for (auto widget : widgets) {
                    subMenu->addChild(widget);
                }
            };
        }
    }

    void createPluginPatches() const
    {
        createPluginPatchLabel();
        auto sources = state->patches.sources;
        auto total = 0;
        for (auto& [name, source] : sources) {
            if (name == "local")
                continue;
            total += source.total;
        }
        if (total == 0)
            return;
        createPluginPatchItems();
    }

    void createPatches() const
    {
        auto sep = createSeparator();
        sep->visibleCallback = [this]() {
            return searchBox->text == "";
        };

        createLocalPatches();
        createPluginPatches();
    }

    /**
     * Library
     */

    void createModuleIndexMenu()
    {
        bool favoritesOnly = !modPressed(RACK_MOD_CTRL);
        auto label = favoritesOnly ? "Favorites:" : "Modules:";
        auto pluginMenu = new LibraryPluginMenu(state, label, favoritesOnly);
        pluginMenu->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(pluginMenu);
    }

    void createModuleTagIndexMenu()
    {
        bool favoritesOnly = !modPressed(RACK_MOD_CTRL);
        auto label = favoritesOnly ? "Favorites by tag:" : "Modules by tag:";
        auto tagIndexMenu = new LibraryTagMenu(state, label, favoritesOnly);
        tagIndexMenu->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(tagIndexMenu);
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
        createSeparator();
        createModuleIndexMenu();
        createModuleTagIndexMenu();
        createModuleResults();

        if (!modPressed(RACK_MOD_CTRL)) {
            auto tip = new ModularMenuLabel();
            tip->text = "Reopen with Ctrl to show all modules";
            tip->visibleCallback = [this]() {
                return searchBox->text == "";
            };
            menu->addChild(tip);
        }
    }

    // main build method
    void build()
    {
        menu = createMenu();
        modules.results.clear();

        createSearchBox();
        createPrefabs();
        createPatches();
        createLibrary();
    }
};