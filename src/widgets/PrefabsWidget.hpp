#pragma once

#include "IconWidget.hpp"
#include "Prefabs.hpp"
#include "ui/LedLabel.hpp"

struct PrefabsWidget : ModuleWidget
{
    LedLabel* prefabsLabel;
    LedLabel* patchesLabel;
    LedLabel* modulesLabel;

    int lastCount = 0;

    PrefabsWidget(Prefabs* module);

    void step() override;
    void draw(const DrawArgs& args) override;
    void appendContextMenu(Menu* menu) override;
};
