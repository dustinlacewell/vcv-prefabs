#include "RackItem.hpp"
#include <library.hpp>
#include <patch.hpp>
#include "RackTooltip.hpp"
#include "clients/library/LibraryClient.hpp"
#include "modules/prefabs/menus/main_menu/modules/LibraryModuleItem.hpp"
#include "modules/prefabs/menus/main_menu/sections/patches/RackContextMenu.hpp"
#include "ui/VerticalGroup.hpp"
#include "ui/menus/ModularMenuItem.hpp"
#include "ui/menus/scrolling/helpers.hpp"
#include "utils/patches.hpp"

RackItem::RackItem(State* state) {
    this->state = state;
    this->text = "Not bound!";
    this->rightText = "";
    this->source = "";
    this->disabled = true;
    this->tooltip = nullptr;
}

RackItem::RackItem(State* state, Rack rack) {
    this->state = state;
    this->text = "Not bound!";
    this->rightText = "";
    this->source = "";
    this->disabled = true;
    this->tooltip = nullptr;
    setRack(rack);
}

void RackItem::setRack(Rack rack) {
    // call other constructor
    this->rack = rack;
    this->text = rack.getDisplayName();
    // warning unicode sign
    this->rightText = rack.isValid ? "" : "⚠";
    this->disabled = false;
    this->source = rack.source;
    this->tooltipCallback = [this]() {
        return createTooltip();
    };
}

void RackItem::unsetRack() {
    this->rack = std::nullopt;
    this->text = "Not bound!";
    this->rightText = "";
    this->source = "";
    this->disabled = true;
    this->tooltipCallback = nullptr;
    this->tooltip = nullptr;
}

Widget* RackItem::createTooltip() {
    if (!rack.has_value()) {
        return nullptr;
    }

    return new RackTooltip(*rack);
}

void RackItem::onButton(const event::Button& e) {
    if (!rack.has_value()) {
        return;
    }

    e.consume(this);

    //    if (this->disabled) {
    //        if (rack->missingModules.size() > 0 && e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
    //            createChildMenu();
    //        }
    //        return;
    //    }

    // get rack.filename's extension
    std::string extension = rack->filename.substr(rack->filename.find_last_of(".") + 1);

    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        // if it's a .vcv file, unarchive it first
        if (extension == "vcv" && !isPatchLegacyV1(rack->filename)) {
            auto patchRoot = asset::user("rack-tmp");
            auto patchPath = patchRoot + '/' + "patch.json";

            system::createDirectories(patchRoot);
            system::unarchiveToDirectory(rack->filename, patchRoot);
            APP->scene->rack->loadSelection(patchPath);
        } else {
            APP->scene->rack->loadSelection(rack->filename);
        }

        for (auto mw : APP->scene->rack->getSelected()) {
            e.consume(mw);
        }
    } else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        createContextMenu();
        return;
    }

    if (e.isConsumed()) {
        // Close menu
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        if (overlay) {
            overlay->requestDelete();
        }
    }
}

void RackItem::createContextMenu() {
    ScrollableMenu* menu = createScrollableMenu();
    auto contextMenu = new RackContextMenu(*rack);
    menu->addMenuItem(contextMenu);
}

std::string RackItem::getRightText() {
    auto text = ModularItem::getRightText();

    if (rack.has_value()) {
        text = rack->isValid ? text : text + "⚠";
    }

    return text;
}

void RackItem::step() {
    if (rack.has_value()) {
        // if the control key is pressed
        if ((APP->window->getMods() & RACK_MOD_MASK) == RACK_MOD_CTRL) {
            text = rack->filename;
        } else {
            text = rack->getDisplayName();
        }
    }

    ModularItem::step();
}

ScrollableMenu* RackItem::createChildMenu() {
    return nullptr;
    // if the rack is not valid, create a child menu offering to download the missing modules
    auto missing = rack->missingModules;
    auto menu = createScrollableMenu();

    auto label = new ModularItem();
    label->text = "Subscribe to missing modules:";
    label->disabled = true;
    menu->addMenuItem(label);

    // add a menu item for each missing module
    for (auto pair : missing) {
        auto pluginSlug = pair.first;
        auto modules = pair.second;
        auto item = new ModularItem();
        item->text = pluginSlug;
        item->rightTextCallback = [this, modules]() {
            return std::to_string(modules.size());
        };
        item->childMenuCallback = [this, pluginSlug, modules](ModularItem* item, ScrollableMenu* menu) {
            for (auto moduleSlug : modules) {
                auto subItem = new ModularMenuItem();
                subItem->text = moduleSlug;
                subItem->buttonCallback = [this, subItem, pluginSlug, moduleSlug](const event::Button& e) {
                    LibraryClient client;
                    client.addModule(pluginSlug, moduleSlug);
                    // remove moduleSlug from missingModules[pluginSlug]
                    this->rack->missingModules[pluginSlug].erase(
                        std::find(this->rack->missingModules[pluginSlug].begin(),
                            this->rack->missingModules[pluginSlug].end(),
                            moduleSlug));
                    subItem->disabled = true;
                    return false;
                };
                menu->addMenuItem(subItem);
            }
        };
        menu->addMenuItem(item);
    }

    return menu;
}