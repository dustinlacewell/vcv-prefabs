#include <rack.hpp>
#include <ui/Slider.hpp>

#include "Prefabs.hpp"
#include "PrefabsWidget.hpp"

#include "IconWidget.hpp"
#include "menus/ModularMenuItem.hpp"
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

    if (module) {
        iconWidget = new IconWidget(module);
        iconWidget->box.pos = module->pos;
        APP->scene->addChildBelow(iconWidget, APP->scene->browser);
    }
}

PrefabsWidget::~PrefabsWidget()
{
    if (iconWidget) {
        iconWidget->requestDelete();
    }
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

    auto total = min(999, prefabsModule->prefabs.total());

    if (total != lastCount) {
        auto totalStr = std::to_string(prefabsModule->prefabs.total());
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

    auto searchResults = new rack::ui::Slider();
    searchResults->box.size.x = 220.0f;
    searchResults->quantity = &prefabsModule->searchResultsQuantity;
    menu->addChild(searchResults);

    auto colorResults = new rack::ui::Slider();
    colorResults->box.size.x = 220.0f;
    colorResults->quantity = &prefabsModule->colorQuantity;
    menu->addChild(colorResults);

    auto discoSpeed = new rack::ui::Slider();
    discoSpeed->box.size.x = 220.0f;
    discoSpeed->quantity = &prefabsModule->discoSpeedQuantity;
    menu->addChild(discoSpeed);

    auto sortMenu = prefabsModule->moduleSorter.createSortTypeMenu();
    menu->addChild(sortMenu);

    auto tagMenu = prefabsModule->tagManager.createToggleMenu();
    menu->addChild(tagMenu);
}