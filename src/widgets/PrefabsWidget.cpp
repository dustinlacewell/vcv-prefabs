#include <rack.hpp>
#include <ui/Slider.hpp>

#include "IconWidget.hpp"
#include "Prefabs.hpp"
#include "PrefabsWidget.hpp"
#include "plugin.h"
#include "ui/LedLabel.hpp"

PrefabsWidget::PrefabsWidget(Prefabs* module)
{
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/Prefabs.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<WhiteLight>>>(
        Vec(box.size.x / 2, box.size.y - 50), module, Prefabs::SHOW_PARAM, Prefabs::STATUS_LIGHT));

    totalLabel = new LedLabel();
    totalLabel->fontSize = 14;
    totalLabel->text = "000";
    totalLabel->box.pos = Vec(12, 35);
    addChild(totalLabel);
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

    auto total = min(999, state->prefabs.total());

    if (total != lastCount) {
        auto totalStr = std::to_string(state->prefabs.total());
        while (totalStr.length() < 3) {
            totalStr = "0" + totalStr;
        }
        totalLabel->text = totalStr;
        lastCount = total;
    }

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