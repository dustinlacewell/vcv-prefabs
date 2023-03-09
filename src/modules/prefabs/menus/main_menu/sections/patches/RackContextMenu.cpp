#include <patch.hpp>

#include "RackContextMenu.hpp"
#include "clients/library/LibraryClient.hpp"
#include "modules/prefabs/menus/main_menu/modules/LibraryModuleItem.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "ui/menus/scrolling/helpers.hpp"

RackContextMenu::RackContextMenu(Rack rack) : rack(rack) {
    std::string extension = rack.filename.substr(rack.filename.find_last_of(".") + 1);
    auto item = new ModularItem();
    item->text = extension == "vcvs" ? "Load as patch" : "Load patch";
    item->buttonCallback = [this](const event::Button& e) {
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        if (overlay) {
            overlay->requestDelete();
        }
        APP->patch->load(this->rack.filename);
        return true;
    };
    addChild(item);

    auto modulesUsedLabel = new ModularItem();
    modulesUsedLabel->text = "Modules used:";
    modulesUsedLabel->disabled = true;
    addChild(modulesUsedLabel);

    // add a menu item for each module used
    for (auto pair : rack.usedPlugins) {
        auto pluginSlug = pair.first;
        auto modules = pair.second;

        auto pluginItem = new ModularItem();
        pluginItem->text = pluginSlug;
        pluginItem->childMenuCallback = [pluginSlug, modules](ModularItem* item, ScrollableMenu* menu) {
            for (auto modulePair : modules) {
                auto moduleSlug = modulePair.first;
                auto uses = modulePair.second;
                auto moduleItem = new LibraryModuleItem(pluginSlug, moduleSlug);
                if (moduleItem->model == nullptr) {
                    moduleItem->text = moduleSlug + " (missing)";
                    moduleItem->disabled = true;

                    moduleItem->rightClickCallback = [pluginSlug, moduleSlug, moduleItem](const event::Button& e) {
                        auto menu = createScrollableMenu();
                        auto subscribeItem = new ModularItem();
                        subscribeItem->text = "Add module";
                        subscribeItem->buttonCallback = [pluginSlug, moduleSlug](const event::Button& e) {
                            LibraryClient client;
                            client.addModule(pluginSlug, moduleSlug);
                            client.checkUpdates();
                            return true;
                        };

                        menu->addMenuItem(subscribeItem);

                        e.consume(moduleItem);
                        return false;
                    };

                    moduleItem->visibleCallback = [moduleItem, pluginSlug, moduleSlug]() {
                        auto whiteListInfo = settings::moduleWhitelist[pluginSlug];
                        auto isSubscribed = false;
                        for (auto _moduleSlug : whiteListInfo.moduleSlugs) {
                            if (_moduleSlug == moduleSlug) {
                                isSubscribed = true;
                                break;
                            }
                        }
                        moduleItem->text = moduleSlug + (isSubscribed ? "" : " (not subscribed)");
                        return true;
                    };
                }
                menu->addMenuItem(moduleItem);
            }
        };
        // add a right click menu for toggling subscription to the plugin
        pluginItem->rightClickCallback = [pluginItem, pluginSlug](const event::Button& e) {
            auto whiteListInfo = settings::moduleWhitelist[pluginSlug];
            auto subscribeMenu = createScrollableMenu();

            auto subscribeItem = new ModularItem();
            subscribeItem->text = "Subscribe to plugin";
            subscribeItem->visibleCallback = [subscribeItem, pluginSlug]() {
                auto whiteListInfo = settings::moduleWhitelist[pluginSlug];
                auto isSubscribed = whiteListInfo.subscribed;
                subscribeItem->text = isSubscribed ? "Unsubscribe from plugin" : "Subscribe to plugin";
                return true;
            };
            subscribeItem->buttonCallback = [pluginSlug](const event::Button& e) {
                auto whiteListInfo = settings::moduleWhitelist[pluginSlug];
                auto isSubscribed = whiteListInfo.subscribed;
                LibraryClient client;
                if (isSubscribed) {
                    client.unsubscribe(pluginSlug);
                    client.checkUpdates();
                } else {
                    client.subscribe(pluginSlug);
                    client.checkUpdates();
                }
                return true;
            };
            subscribeMenu->addMenuItem(subscribeItem);

            e.consume(pluginItem);
            return false;
        };

        addChild(pluginItem);
    }
}