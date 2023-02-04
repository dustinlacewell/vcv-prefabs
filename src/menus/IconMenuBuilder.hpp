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
#include "menus/prefabs/GroupItem.hpp"
#include "menus/prefabs/PluginItem.hpp"
#include "menus/prefabs/RackItem.hpp"
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

    //    PrefabSource& getLocalPrefabSource() const { return state->store.prefabs.getLocalSource(); }
    //    PrefabSource& getLocalPatchSource() const { return state->store.patches.getLocalSource(); }

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
        auto sep = createSeparator();
        sep->visibleCallback = [this]() {
            return searchBox->text != "";
        };

        auto label = new ModularMenuLabel();
        label->text = "Prefabs";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        auto sep2 = createSeparator();
        sep2->visibleCallback = [this]() {
            return searchBox->text != "";
        };

        for (const auto& sourcePair : state->prefabsIndex.sources) {
            auto slug = sourcePair.first;
            auto source = sourcePair.second;

            for (const auto& racks : source.racks) {
                auto item = new RackItem(state, racks);
                item->source = slug;
                item->visibleCallback = [this, racks]() {
                    bool nonEmpty = this->searchBox->text != "";
                    bool found = racks.name.find(this->searchBox->text) != std::string::npos;
                    return nonEmpty && found;
                };
                menu->addChild(item);
            }
        }
    }

    void createPatchResults() const
    {
        auto sep = createSeparator();
        sep->visibleCallback = [this]() {
            return searchBox->text != "";
        };

        auto label = new ModularMenuLabel();
        label->text = "Patches";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        auto sep2 = createSeparator();
        sep2->visibleCallback = [this]() {
            return searchBox->text != "";
        };

        auto& localSource = state->patchesIndex.sources["local"];
        for (const auto& rack : localSource.racks) {
            auto item = new RackItem(state, rack);
            item->source = "local";
            item->visibleCallback = [this, rack]() {
                bool nonEmpty = this->searchBox->text != "";
                bool found = rack.name.find(this->searchBox->text) != std::string::npos;
                return nonEmpty && found;
            };
            menu->addChild(item);
        }

        for (const auto& sourcePair : state->patchesIndex.sources) {
            auto slug = sourcePair.first;
            auto source = sourcePair.second;

            if (slug == "local") {
                continue;
            }

            for (const auto& rack : source.racks) {
                auto item = new RackItem(state, rack);
                item->source = slug;
                item->visibleCallback = [this, rack]() {
                    bool nonEmpty = this->searchBox->text != "";
                    bool found = rack.name.find(this->searchBox->text) != std::string::npos;
                    return nonEmpty && found;
                };
                menu->addChild(item);
            }
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
        //        auto& localSource = state->store.prefabs.getLocalSource();
        auto& localSource = state->prefabsIndex.sources["local"];

        auto untagged = localSource.groups.find("untagged");
        if (untagged != localSource.groups.end()) {
            makeDefault(new GroupItem(state, "untagged", untagged->second));
        }
    }

    void createLocalPrefabTags() const
    {
        auto& localSource = state->prefabsIndex.sources["local"];
        for (auto& [groupName, groupPrefabs] : localSource.groups) {
            if (groupName == "untagged")
                continue;
            makeDefault(new GroupItem(state, groupName, groupPrefabs));
        }

        createLocalPrefabUntagged();
    }

    void createLocalPrefabTagsByModule() const
    {
        auto& localSource = state->prefabsIndex.sources["local"];
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

    auto createPrefabSourceMenu(const SourceIndex source) const
    {
        std::vector<Widget*> widgets = {};

        // tagged
        for (auto& [groupName, groupPrefabs] : source.groups) {
            if (groupName == "untagged")
                continue;
            auto tag = new GroupItem(state, groupName, groupPrefabs);
            widgets.push_back(tag);
        }

        // untagged
        auto untagged = source.groups.find("untagged");
        if (untagged != source.groups.end()) {
            auto tag = new GroupItem(state, "untagged", untagged->second);
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
        for (const auto& source : state->prefabsIndex.sources) {
            const auto& sourceName = source.first;
            const auto& sourceIndex = source.second;

            if (sourceName == "local") {
                continue;
            }

            if (sourceIndex.racks.size() == 0)
                continue;

            auto item = makeDefault(new ModularMenuItem());
            item->text = sourceName;
            item->childMenuCallback = [this, sourceIndex](ModularMenuItem* item, Menu* subMenu) {
                auto widgets = createPrefabSourceMenu(sourceIndex);
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
        auto& localSource = state->patchesIndex.sources["local"];

        auto untagged = localSource.groups.find("untagged");
        if (untagged != localSource.groups.end()) {
            makeDefault(new GroupItem(state, "untagged", untagged->second));
        }
    }

    void createLocalPatchTags() const
    {
        auto& localSource = state->patchesIndex.sources["local"];
        for (auto& [groupName, tagPatches] : localSource.groups) {
            if (groupName == "untagged")
                continue;
            makeDefault(new GroupItem(state, groupName, tagPatches));
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

    auto createPatchSourceMenu(const SourceIndex source) const
    {
        if (source.racks.empty()) {
            return std::vector<Widget*>{};
        }

        std::vector<Widget*> widgets = {};

        // tagged
        for (auto& [groupName, tagPatches] : source.groups) {
            if (groupName == "untagged")
                continue;
            auto tag = new GroupItem(state, groupName, tagPatches);
            widgets.push_back(tag);
        }

        // untagged
        auto untagged = source.groups.find("untagged");
        if (untagged != source.groups.end()) {
            auto tag = new GroupItem(state, "untagged", untagged->second);
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
        for (const auto& source : state->patchesIndex.sources) {
            const auto& sourceName = source.first;
            const auto& sourceIndex = source.second;

            if (sourceName == "local") {
                continue;
            }

            if (sourceIndex.racks.empty())
                continue;

            auto item = makeDefault(new ModularMenuItem());
            item->text = sourceName;
            item->childMenuCallback = [this, sourceIndex](ModularMenuItem* item, Menu* subMenu) {
                auto widgets = createPrefabSourceMenu(sourceIndex);
                for (auto widget : widgets) {
                    subMenu->addChild(widget);
                }
            };
        }
    }

    void createPluginPatches() const
    {
        createPluginPatchLabel();
        auto sources = state->store.patchSources;
        auto total = 0;
        for (auto source : sources) {
            if (source->getSlug() == "local")
                continue;
            total += source->getTotal();
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
        createPatchResults();
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
        auto label = favoritesOnly ? "Favorites by group:" : "Modules by group:";
        auto tagIndexMenu = new LibraryTagMenu(state, label, favoritesOnly);
        tagIndexMenu->visibleCallback = [this]() {
            return searchBox->text == "";
        };
        menu->addChild(tagIndexMenu);
    }

    void createModuleResults()
    {
        auto label = new ModularMenuLabel();
        label->text = "Modules";
        label->visibleCallback = [this]() {
            return searchBox->text != "";
        };
        menu->addChild(label);

        auto sep = createSeparator();
        sep->visibleCallback = [this]() {
            return searchBox->text != "";
        };

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