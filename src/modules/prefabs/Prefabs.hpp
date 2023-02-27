#pragma once

#include <rack.hpp>
#include "models/ModuleSorter.hpp"
#include "models/ModuleTagManager.hpp"
#include "models/SimpleQuantity.hpp"
#include "modules/prefabs/ui/IconWidget.hpp"

using namespace rack;

struct Prefabs : rack::Module {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId { OUTPUTS_LEN };
    enum LightId { WIDGET_LIGHT, REFRESH_LIGHT, LIGHTS_LEN };

    IconWidget* widget;

    Prefabs();

    void process(const ProcessArgs& args) override;
    IconWidget* findWidget();
    void enableBrowserMode();
    void disableBrowserMode();
};