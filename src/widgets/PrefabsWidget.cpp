// to avoid undefined reference to `glGenTextures'

#include <rack.hpp>
#include <ui/Slider.hpp>

#include "IconWidget.hpp"
#include "Prefabs.hpp"
#include "PrefabsWidget.hpp"
#include "plugin.h"
#include "ui/LedLabel.hpp"
#include "ui/WidgetToggle.hpp"

PrefabsWidget::PrefabsWidget(Prefabs* module)
{
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/Prefabs2.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    auto center = box.size / 2.0;

    auto centerWidget = [=](Widget* w, Vec center) {
        w->box.pos = Vec(center.x - w->box.size.x / 2, center.y - w->box.size.y / 2);
    };

    // button for toggling the widget / refreshing
    //    auto toggle = new WidgetToggle(module);
    //    addChild(toggle);
    //    centerWidget(toggle, Vec(center.x, box.size.y - 50));

    // add small green light to show that the module is active
    addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(7.5, 43)), module, Prefabs::REFRESH_LIGHT));

    prefabsLabel = new LedLabel();
    prefabsLabel->fontSize = 14;
    prefabsLabel->text = "0000";
    prefabsLabel->box.pos = Vec(0, 36);
    prefabsLabel->box.size = Vec(box.size.x, 20);
    prefabsLabel->tooltipCallback = [=]() {
        auto tooltip = new Tooltip();
        tooltip->text = "Total prefabs";
        return tooltip;
    };
    addChild(prefabsLabel);

    patchesLabel = new LedLabel();
    patchesLabel->fontSize = 14;
    patchesLabel->text = "0000";
    patchesLabel->box.pos = Vec(0, 64);
    patchesLabel->box.size = Vec(box.size.x, 20);
    patchesLabel->tooltipCallback = [=]() {
        auto tooltip = new Tooltip();
        tooltip->text = "Total patches";
        return tooltip;
    };
    addChild(patchesLabel);

    modulesLabel = new LedLabel();
    modulesLabel->fontSize = 14;
    modulesLabel->text = "0000";
    modulesLabel->box.pos = Vec(0, 94);
    modulesLabel->box.size = Vec(box.size.x, 20);
    modulesLabel->tooltipCallback = [=]() {
        auto tooltip = new Tooltip();
        tooltip->text = "Total modules";
        return tooltip;
    };
    addChild(modulesLabel);
}

void PrefabsWidget::step()
{
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

    auto prefabsTotal = min(9999, state->store.getTotalPrefabs());
    auto prefabsTotalStr = std::to_string(prefabsTotal);
    while (prefabsTotalStr.length() < 4) {
        prefabsTotalStr = "0" + prefabsTotalStr;
    }
    prefabsLabel->text = prefabsTotalStr;

    auto patchesTotal = min(9999, state->store.getTotalPatches());
    auto patchesTotalStr = std::to_string(patchesTotal);
    while (patchesTotalStr.length() < 4) {
        patchesTotalStr = "0" + patchesTotalStr;
    }
    patchesLabel->text = patchesTotalStr;

    auto modulesTotal = 0;
    for (auto plugin : rack::plugin::plugins) {
        modulesTotal += plugin->models.size();
    }
    modulesTotal = min(9999, modulesTotal);
    auto modulesTotalStr = std::to_string(modulesTotal);
    while (modulesTotalStr.length() < 4) {
        modulesTotalStr = "0" + modulesTotalStr;
    }
    modulesLabel->text = modulesTotalStr;

    ModuleWidget::step();
}

void PrefabsWidget::draw(const DrawArgs& args)
{
    ModuleWidget::draw(args);
}

void PrefabsWidget::appendContextMenu(Menu* menu)
{
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
    browserMode->rightText = CHECKMARK(state->browserMode);
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
                }
                else {
                    prefabsModule->disableBrowserMode();
                }
            }
        }

        e.consume(browserMode);
        return false;
    };
    menu->addChild(browserMode);

    auto searchResults = new rack::ui::Slider();
    searchResults->box.size.x = 220.0f;
    searchResults->quantity = &state->searchResultsQuantity;
    menu->addChild(searchResults);

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
}