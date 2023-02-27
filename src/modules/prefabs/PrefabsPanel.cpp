// to avoid undefined reference to `glGenTextures'

#include <rack.hpp>
#include <ui/Slider.hpp>

#include "PrefabsPanel.hpp"
#include "modules/prefabs/Prefabs.hpp"
#include "modules/prefabs/ui/IconWidget.hpp"
#include "ui/menus/ModularMenuItem.hpp"

PrefabsPanel::PrefabsPanel(Prefabs* module) : PanelWidget<Prefabs>(module, "res/Prefabs.svg") {
    makeLight("refresh_light", "Lit when refreshing", Prefabs::REFRESH_LIGHT);
    makeToggle(
        "toggle_button",
        "Toggle icon widget",
        [module]() {
            if (!module) {
                return false;
            }

            if (!module->widget) {
                return false;
            }

            if (!module->widget->state) {
                return false;
            }

            return module->widget->state->showing;
        },
        [module]() {
            if (module && module->widget && module->widget->state) {
                module->widget->state->showing = true;
                module->widget->state->reload();
            }
        },
        [module]() {
            if (module && module->widget && module->widget->state) {
                module->widget->state->showing = false;
            }
        });

    prefabsLabel = makeLabel("led_1", "Total prefabs");
    patchesLabel = makeLabel("led_2", "Total patches");
    modulesLabel = makeLabel("led_3", "Total modules");
}

void PrefabsPanel::step() {
    if (!module) {
        return;
    }

    Prefabs* prefabsModule = dynamic_cast<Prefabs*>(module);

    if (!prefabsModule) {
        return;
    }

    auto widget = prefabsModule->widget;

    if (!widget) {
        return;
    }

    auto state = prefabsModule->widget->state;

    if (prefabsLabel.has_value()) {
        auto prefabsTotal = min(9999, state->store.getTotalPrefabs());
        auto prefabsTotalStr = std::to_string(prefabsTotal);
        while (prefabsTotalStr.length() < 4) {
            prefabsTotalStr = "0" + prefabsTotalStr;
        }
        (*prefabsLabel)->text = prefabsTotalStr;
    }

    if (patchesLabel.has_value()) {
        auto patchesTotal = min(9999, state->store.getTotalPatches());
        auto patchesTotalStr = std::to_string(patchesTotal);
        while (patchesTotalStr.length() < 4) {
            patchesTotalStr = "0" + patchesTotalStr;
        }
        (*patchesLabel)->text = patchesTotalStr;
    }

    if (modulesLabel.has_value()) {
        auto modulesTotal = 0;
        for (auto plugin : rack::plugin::plugins) {
            modulesTotal += plugin->models.size();
        }
        modulesTotal = min(9999, modulesTotal);
        auto modulesTotalStr = std::to_string(modulesTotal);
        while (modulesTotalStr.length() < 4) {
            modulesTotalStr = "0" + modulesTotalStr;
        }
        (*modulesLabel)->text = modulesTotalStr;
    }

    ModuleWidget::step();
}

void PrefabsPanel::draw(const DrawArgs& args) {
    ModuleWidget::draw(args);
}

void PrefabsPanel::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSeparator());

    Prefabs* prefabsModule = dynamic_cast<Prefabs*>(module);

    if (!prefabsModule) {
        return;
    }

    auto widget = prefabsModule->widget;

    if (!widget) {
        return;
    }

    auto state = prefabsModule->widget->state;

    if (state == nullptr) {
        return;
    }

    auto browserMode = new ModularMenuItem();
    browserMode->text = "Replace browser";
    browserMode->rightTextCallback = [=]() {
        return CHECKMARK(state->browserMode);
    };
    browserMode->visibleCallback = [=]() {
        browserMode->rightText = CHECKMARK(state->browserMode);
        return true;
    };
    browserMode->buttonCallback = [=](const event::Button& e) {
        if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
            state->browserMode = !state->browserMode;
            Prefabs* prefabsModule = dynamic_cast<Prefabs*>(module);
            if (prefabsModule) {
                if (state->browserMode) {
                    prefabsModule->enableBrowserMode();
                } else {
                    prefabsModule->disableBrowserMode();
                }
            }
        }

        e.consume(browserMode);
        return false;
    };
    menu->addChild(browserMode);

    auto layout = new ModularMenuItem();
    layout->text = "Layout";

    layout->childMenuCallback = [prefabsModule](ModularMenuItem* item, Menu* subMenu) {
        // create a subitem for each layout
        auto stackedItem = new ModularMenuItem();
        stackedItem->text = "Stacked";
        stackedItem->rightTextCallback = [state = prefabsModule->widget->state]() {
            return CHECKMARK(state->layout == LayoutType::STACKED);
        };
        stackedItem->buttonCallback = [stackedItem, state = prefabsModule->widget->state](const event::Button& e) {
            if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
                state->layout = LayoutType::STACKED;
            }

            e.consume(stackedItem);
            return false;
        };
        subMenu->addChild(stackedItem);

        // create a subitem for each layout
        auto tabbedItem = new ModularMenuItem();
        tabbedItem->text = "Tabbed";
        tabbedItem->rightTextCallback = [state = prefabsModule->widget->state]() {
            return CHECKMARK(state->layout == LayoutType::TABBED);
        };
        tabbedItem->buttonCallback = [tabbedItem, state = prefabsModule->widget->state](const event::Button& e) {
            if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
                state->layout = LayoutType::TABBED;
            }

            e.consume(tabbedItem);
            return false;
        };
        subMenu->addChild(tabbedItem);
    };
    menu->addChild(layout);

    auto searchResults = new rack::ui::Slider();
    searchResults->box.size.x = 220.0f;
    searchResults->quantity = &state->searchResultsQuantity;
    menu->addChild(searchResults);

    auto monoIcon = new ModularMenuItem();
    monoIcon->text = "Monochrome icon";
    monoIcon->rightTextCallback = [=]() {
        return CHECKMARK(state->monoIcon);
    };
    monoIcon->visibleCallback = [=]() {
        monoIcon->rightText = CHECKMARK(state->monoIcon);
        return true;
    };
    monoIcon->buttonCallback = [=](const event::Button& e) {
        if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
            state->monoIcon = !state->monoIcon;
        }

        e.consume(monoIcon);
        return false;
    };
    menu->addChild(monoIcon);

    auto colorResults = new rack::ui::Slider();
    colorResults->box.size.x = 220.0f;
    colorResults->quantity = &state->colorQuantity;
    menu->addChild(colorResults);

    auto discoSpeed = new rack::ui::Slider();
    discoSpeed->box.size.x = 220.0f;
    discoSpeed->quantity = &state->discoSpeedQuantity;
    menu->addChild(discoSpeed);

    auto sortMenu = state->moduleSorter.createSortTypeMenu();
    menu->addChild(sortMenu);

    auto tagMenu = state->tagManager.createToggleMenu();
    menu->addChild(tagMenu);

    auto refreshMenu = new ModularMenuItem();
    refreshMenu->text = "Refresh";
    refreshMenu->visibleCallback = [=]() {
        if (refreshing) {
            refreshMenu->text = "Refreshing...";
        } else {
            refreshMenu->text = "Refresh";
        }
        return true;
    };
    refreshMenu->buttonCallback = [=](const event::Button& e) {
        if (refreshing) {
            return false;
        }
        if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
            state->store.refresh();
        }

        e.consume(refreshMenu);
        return true;
    };
    menu->addChild(refreshMenu);
}