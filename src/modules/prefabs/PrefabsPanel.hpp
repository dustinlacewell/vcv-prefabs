#pragma once

#include "SvgHelper.hpp"

#include "modules/prefabs/Prefabs.hpp"
#include "modules/prefabs/ui/IconWidget.hpp"
#include "ui/LedLabel.hpp"
#include "ui/PanelWidget.hpp"

template struct PanelWidget<Prefabs>;

struct PrefabsPanel : PanelWidget<Prefabs> {
    std::optional<LedLabel*> prefabsLabel;
    std::optional<LedLabel*> patchesLabel;
    std::optional<LedLabel*> modulesLabel;

    int lastCount = 0;

    PrefabsPanel(Prefabs* module);

    void step() override;
    void draw(const DrawArgs& args) override;
    void appendContextMenu(Menu* menu) override;
};