#pragma once

#include "IconWidget.hpp"
#include "Prefabs.hpp"
#include "ui/LedLabel.hpp"

struct PrefabsWidget : ModuleWidget
{
    IconWidget* iconWidget {nullptr};
    LedLabel* totalLabel;

    int lastCount = 0;

    PrefabsWidget(Prefabs* module);
    ~PrefabsWidget();

    void step() override;
    void draw(const DrawArgs& args) override;
    void appendContextMenu(Menu* menu) override;
};
