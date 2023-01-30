#include <dirent.h>
#include <sys/stat.h>
#include <rack.hpp>

#include "Prefabs.hpp"
#include "widgets/PrefabsWidget.hpp"

using namespace rack::dsp;

Prefabs::Prefabs()
{
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(STATUS_LIGHT, "Status light");

    searchResultsQuantity = SimpleQuantity();
    searchResultsQuantity.label = "Number of search results";
    searchResultsQuantity.minValue = 8;
    searchResultsQuantity.maxValue = 128;
    searchResultsQuantity.defaultValue = 32;
    searchResultsQuantity.value = 32;
    searchResultsQuantity.rounded = true;

    colorQuantity = SimpleQuantity();
    colorQuantity.label = "Icon color";
    colorQuantity.minValue = 0;
    colorQuantity.maxValue = 1;
    colorQuantity.defaultValue = .25;
    colorQuantity.value = .25;
    colorQuantity.rounded = false;

    discoSpeedQuantity = SimpleQuantity();
    discoSpeedQuantity.label = "Disco speed";
    discoSpeedQuantity.minValue = 0.0f;
    discoSpeedQuantity.maxValue = 0.03f;
    discoSpeedQuantity.defaultValue = 0.0;
    discoSpeedQuantity.value = 0.0;
    discoSpeedQuantity.rounded = false;

    prefabs = PrefabStore();
    prefabs.refresh();
}

void Prefabs::show()
{
    prefabs.refresh();
    this->showing = true;
    lights[STATUS_LIGHT].setBrightness(1.0f);
    params[SHOW_PARAM].setValue(1.0f);
}

void Prefabs::hide()
{
    this->showing = false;
    lights[STATUS_LIGHT].setBrightness(0.0f);
    params[SHOW_PARAM].setValue(0.0f);
}

void Prefabs::toggle()
{
    if (this->showing) {
        this->hide();
    }
    else {
        this->show();
    }
}

void Prefabs::process(const ProcessArgs& args)
{
    bool shouldShow = params[SHOW_PARAM].getValue() > 0.5f;
    if (this->showing && !shouldShow) {
        this->hide();
    }
    else if (!this->showing && shouldShow) {
        this->show();
    }
}

json_t* Prefabs::dataToJson()
{
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "showing", json_boolean(showing));
    json_object_set_new(rootJ, "pos", json_pack("[f, f]", pos.x, pos.y));
    json_object_set_new(rootJ, "searchResultsQuantity", searchResultsQuantity.toJson());
    json_object_set_new(rootJ, "colorQuantity", colorQuantity.toJson());
    json_object_set_new(rootJ, "discoSpeedQuantity", discoSpeedQuantity.toJson());
    return rootJ;
}

void Prefabs::dataFromJson(json_t* rootJ)
{
    json_t* showJ = json_object_get(rootJ, "showing");
    if (showJ)
        showing = json_boolean_value(showJ);

    json_t* posJ = json_object_get(rootJ, "pos");
    if (posJ)
        pos = rack::Vec(json_real_value(json_array_get(posJ, 0)), json_real_value(json_array_get(posJ, 1)));

    if (showing) {
        this->show();
    }
    else {
        this->hide();
    }

    json_t* searchResultsQuantityJ = json_object_get(rootJ, "searchResultsQuantity");
    if (searchResultsQuantityJ)
        searchResultsQuantity.fromJson(searchResultsQuantityJ);

    json_t* colorQuantityJ = json_object_get(rootJ, "colorQuantity");
    if (colorQuantityJ)
        colorQuantity.fromJson(colorQuantityJ);

    json_t* discoSpeedQuantityJ = json_object_get(rootJ, "discoSpeedQuantity");
    if (discoSpeedQuantityJ)
        discoSpeedQuantity.fromJson(discoSpeedQuantityJ);
}

Model* modelPrefabs = createModel<Prefabs, PrefabsWidget>("Prefabs");
