#pragma once

#include <rack.hpp>
#include "models/ModuleSorter.hpp"
#include "models/ModuleTagManager.hpp"
#include "models/PatchStore.hpp"
#include "models/Prefab.hpp"
#include "models/PrefabStore.hpp"
#include "models/SimpleQuantity.hpp"
#include "widgets/IconWidget.hpp"

using namespace rack;

struct Prefabs : rack::Module
{
    enum ParamId
    {
        SHOW_PARAM,
        PARAMS_LEN
    };
    enum InputId
    {
        INPUTS_LEN
    };
    enum OutputId
    {
        OUTPUTS_LEN
    };
    enum LightId
    {
        STATUS_LIGHT,
        LIGHTS_LEN
    };

    float lastShowParam;

    IconWidget* widget;

    Prefabs();

    void process(const ProcessArgs& args) override;
    IconWidget* findWidget();
    void enableBrowserMode();
    void disableBrowserMode();
};