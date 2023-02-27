#include <rack.hpp>

#include "FacadePanel.hpp"
#include "utils/logging.hpp"

using namespace rack::dsp;

Facade::Facade() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
}

// polyphonic version of process
void Facade::process(const ProcessArgs& args) {
    for (int i = 0; i < INPUTS_LEN; i++) {
        int channels = inputs[IN_1 + i].getChannels();
        outputs[OUT_1 + i].setChannels(channels);
        for (int ch = 0; ch < channels; ch++) {
            outputs[OUT_1 + i].setVoltage(inputs[IN_1 + i].getVoltage(ch), ch);
        }
    }
}

json_t* Facade::dataToJson() {
    json_t* rootJ = json_object();

    json_t* labelsJ = json_array();
    for (int i = 0; i < 8; i++) {
        json_t* labelJ = json_string(labels[i].c_str());
        json_array_append_new(labelsJ, labelJ);
    }
    json_object_set_new(rootJ, "labels", labelsJ);

    return rootJ;
}

void Facade::dataFromJson(json_t* rootJ) {
    json_t* labelsJ = json_object_get(rootJ, "labels");
    if (labelsJ) {
        for (int i = 0; i < 8; i++) {
            json_t* labelJ = json_array_get(labelsJ, i);
            if (labelJ) {
                labels[i] = json_string_value(labelJ);
            }
        }
    }
}

Model* modelFacade = createModel<Facade, FacadePanel>("Facade");
