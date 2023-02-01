#pragma once

#include <rack.hpp>
#include "models/ModuleSorter.hpp"
#include "models/ModuleTagManager.hpp"
#include "models/PatchStore.hpp"
#include "models/Prefab.hpp"
#include "models/PrefabStore.hpp"
#include "models/SimpleQuantity.hpp"

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

    bool showing = true;
    Vec pos = Vec(30, 30);

    ModuleTagManager tagManager;
    ModuleSorter moduleSorter;

    PrefabStore prefabs;
    PatchStore patches;

    SimpleQuantity searchResultsQuantity;
    SimpleQuantity colorQuantity;
    SimpleQuantity discoSpeedQuantity;

    Prefabs();

    void show();
    void hide();
    void toggle();

    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;

    void process(const ProcessArgs& args) override;
};