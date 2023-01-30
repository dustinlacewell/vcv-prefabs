#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "ModularMenuLabel.hpp"
#include "PluginItem.hpp"
#include "PrefabItem.hpp"
#include "TagItem.hpp"
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
            makeDefault(new TagItem(module, tagName, tagPrefabs));
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
            auto tag = new TagItem(module, tagName, tagPrefabs);
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

    void createModuleSearchResult(int index)
    {
        auto item = new ModularMenuItem();
        item->visibleCallback = [this, item, index]() {
            auto size = modules.results.size();
            auto maxSize = module->searchResultsQuantity.getValue();
            auto limit = fmin(size, maxSize);

            if (index < limit) {
                auto model = modules.results[index];
                if (item->text != model->name) {
                    item->text = model->name;
                    item->rightText = model->plugin->name;

                    auto modelbox = new ModelBox();
                    modelbox->setModel(model);
                    modelbox->zoom = 0.8f;
                    modelbox->updateZoom();

                    auto tooltip = new Tooltip();
                    tooltip->addChild(modelbox);
                    tooltip->box.size = modelbox->box.size;

                    item->setTooltip(tooltip);
                }
                return true;
            }
            return false;
        };
        item->buttonCallback = [this, index](const event::Button& e) {
            auto size = modules.results.size();
            auto maxSize = module->searchResultsQuantity.getValue();
            auto limit = fmin(size, maxSize);

            if (index >= limit) {
                return false;
            }

            auto model = modules.results[index];
            auto newModule = model->createModule();
            APP->engine->addModule(newModule);
            auto widget = model->createModuleWidget(newModule);
            APP->scene->rack->updateModuleOldPositions();
            APP->scene->rack->addModuleAtMouse(widget);
            e.consume(widget);
            return true;
        };
        menu->addChild(item);
    }

    void createModuleSearchResults()
    {
        for (int i = 0; i < 128; i++) {
            createModuleSearchResult(i);
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

    void createModuleIndexModuleSubMenuItem(Model* pluginModule, Menu* pluginSubMenu) const
    {
        auto moduleItem = new ModularMenuItem();
        moduleItem->text = pluginModule->name;

        moduleItem->buttonCallback = [pluginModule](const event::Button& e) {
            auto newModule = pluginModule->createModule();
            APP->engine->addModule(newModule);
            auto widget = pluginModule->createModuleWidget(newModule);
            APP->scene->rack->updateModuleOldPositions();
            APP->scene->rack->addModuleAtMouse(widget);
            e.consume(widget);
            return true;
        };
        // right click to show context menu
        moduleItem->rightClickCallback = [pluginModule](const event::Button& e) {
            auto overlay = new MenuOverlay();
            auto contextMenu = new Menu();
            overlay->addChild(contextMenu);

            // add item to toggle favorite
            auto favoriteItem = new ModularMenuItem();
            favoriteItem->text = "Favorite";
            favoriteItem->rightText = CHECKMARK(pluginModule->isFavorite());
            favoriteItem->buttonCallback = [pluginModule](const event::Button& e) {
                pluginModule->setFavorite(!pluginModule->isFavorite());
                e.consume(NULL);
                return true;
            };
            contextMenu->addChild(favoriteItem);

            contextMenu->box.pos = APP->scene->mousePos;
            APP->scene->addChild(overlay);
            e.consume(contextMenu);
            return true;
        };
        moduleItem->visibleCallback = [pluginModule, moduleItem]() {
            moduleItem->rightText = modPressed(RACK_MOD_CTRL) ? CHECKMARK(pluginModule->isFavorite()) : "";
            return modPressed(RACK_MOD_CTRL) || pluginModule->isFavorite();
        };
        moduleItem->tooltipCallback = [pluginModule]() {
            auto modelbox = new ModelBox();
            modelbox->setModel(pluginModule);
            modelbox->createPreview();
            modelbox->zoom = 0.8f;
            modelbox->updateZoom();

            std::vector<std::string> tagAliases;

            for (auto& tagId : pluginModule->tagIds) {
                for (const std::string& tagAlias : tag::tagAliases[tagId]) {
                    tagAliases.push_back(tagAlias);
                }
            }
            std::string tagStr = rack::string::join(tagAliases, ", ");
            std::transform(tagStr.begin(), tagStr.end(), tagStr.begin(), ::tolower);

            auto group = new VerticalGroup();

            group->addChild(modelbox);

            auto tagLabel = new Label();
            group->addChild(tagLabel);
            tagLabel->text = tagStr;
            Rect r;
            modelbox->zoomWidget->getViewport(r);
            tagLabel->box.size.x = fmax(300, r.getWidth());

            return group;
        };

        pluginSubMenu->addChild(moduleItem);
    }

    void createModuleIndexModuleSubMenu(Plugin* plugin, Menu* pluginSubMenu) const
    {
        for (auto pluginModule : plugin->models) {
            createModuleIndexModuleSubMenuItem(pluginModule, pluginSubMenu);
        }
    };

    void createModuleIndexSubMenuItem(Plugin*& plugin, Menu* indexSubMenu) const
    {
        auto pluginItem = new ModularMenuItem();
        pluginItem->text = plugin->slug;
        bool hasFavorite = false;
        for (auto pluginModule : plugin->models) {
            if (pluginModule->isFavorite()) {
                hasFavorite = true;
                break;
            }
        }
        pluginItem->visibleCallback = [hasFavorite]() {
            return modPressed(RACK_MOD_CTRL) || hasFavorite;
        };
        pluginItem->childMenuCallback = [this, plugin](ModularMenuItem* item, Menu* pluginSubMenu) {
            createModuleIndexModuleSubMenu(plugin, pluginSubMenu);
        };
        indexSubMenu->addChild(pluginItem);
    }

    void createModuleIndexSubMenu(Menu* indexSubMenu)
    {
        for (auto& plugin : rack::plugin::plugins) {
            createModuleIndexSubMenuItem(plugin, indexSubMenu);
        }
    }

    void createModuleIndexMenu()
    {
        auto all = new ModularMenuItem();
        all->text = "All Rack modules:";
        all->visibleCallback = [this, all]() {
            auto ctrl = modPressed(RACK_MOD_CTRL);
            all->text = ctrl ? "All modules:" : "Favorite modules:";
            return searchBox->text == "";
        };
        all->childMenuCallback = [this](ModularMenuItem* item, Menu* indexSubMenu) {
            createModuleIndexSubMenu(indexSubMenu);
        };
        menu->addChild(all);
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
        createSeparator();
        createModuleResults();
        createModuleIndexMenu();
    }
};