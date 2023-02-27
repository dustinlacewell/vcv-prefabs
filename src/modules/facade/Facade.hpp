#pragma once

#include <rack.hpp>

using namespace rack;

struct Facade : rack::Module {
    // array of 8 std::string
    std::string labels[8];

    enum ParamId { PARAMS_LEN };
    enum InputId { IN_1, IN_2, IN_3, IN_4, IN_5, IN_6, IN_7, IN_8, INPUTS_LEN };
    enum OutputId { OUT_1, OUT_2, OUT_3, OUT_4, OUT_5, OUT_6, OUT_7, OUT_8, OUTPUTS_LEN };
    enum LightId { LIGHTS_LEN };

    Facade();

    void process(const ProcessArgs& args) override;
    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;
};