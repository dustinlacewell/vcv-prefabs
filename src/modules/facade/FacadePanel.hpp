#pragma once

#include "Facade.hpp"
#include "modules/prefabs/ui/IconWidget.hpp"
#include "ui/LedLabel.hpp"
#include "ui/PanelWidget.hpp"

struct FacadePanel : PanelWidget<Facade> {
    FacadePanel(Facade* module);
};
