#include <rack.hpp>

#include "State.hpp"
#include "utils/logging.hpp"

using namespace rack::dsp;

State::State()
{
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

    patches = PatchStore();
    patches.refresh();

    tagManager = ModuleTagManager();
}

json_t* State::toJson()
{
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "showing", json_boolean(showing));
    json_object_set_new(rootJ, "pos", json_pack("[f, f]", pos.x, pos.y));
    json_object_set_new(rootJ, "searchResultsQuantity", searchResultsQuantity.toJson());
    json_object_set_new(rootJ, "colorQuantity", colorQuantity.toJson());
    json_object_set_new(rootJ, "discoSpeedQuantity", discoSpeedQuantity.toJson());
    json_object_set_new(rootJ, "tagManager", tagManager.toJson());
    return rootJ;
}

void State::fromJson(json_t* rootJ)
{
    json_t* showJ = json_object_get(rootJ, "showing");
    if (showJ)
        showing = json_boolean_value(showJ);

    json_t* posJ = json_object_get(rootJ, "pos");
    if (posJ)
        pos = rack::Vec(json_real_value(json_array_get(posJ, 0)), json_real_value(json_array_get(posJ, 1)));

    json_t* searchResultsQuantityJ = json_object_get(rootJ, "searchResultsQuantity");
    if (searchResultsQuantityJ)
        searchResultsQuantity.fromJson(searchResultsQuantityJ);

    json_t* colorQuantityJ = json_object_get(rootJ, "colorQuantity");
    if (colorQuantityJ)
        colorQuantity.fromJson(colorQuantityJ);

    json_t* discoSpeedQuantityJ = json_object_get(rootJ, "discoSpeedQuantity");
    if (discoSpeedQuantityJ)
        discoSpeedQuantity.fromJson(discoSpeedQuantityJ);

    json_t* tagManagerJ = json_object_get(rootJ, "tagManager");
    if (tagManagerJ)
        tagManager.fromJson(tagManagerJ);
}

void State::save()
{
    auto path = asset::user("prefabs.json");
    FILE* file = fopen(path.c_str(), "w");
    if (!file) {
        DINFO("[Prefabs] When saving Settings, could not open %s", path.c_str());
        return;
    }

    json_t* rootJ = toJson();
    json_dumpf(rootJ, file, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
    fclose(file);
    json_decref(rootJ);

    DINFO("[Prefabs] Saved Settings to %s", path.c_str());
}

void State::load()
{
    auto path = asset::user("prefabs.json");
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        DINFO("[Prefabs] When loading Settings, could not open %s", path.c_str());
        return;
    }

    json_error_t error;
    json_t* rootJ = json_loadf(file, 0, &error);
    if (!rootJ) {
        if (settings::devMode) {
            DINFO("[Prefabs] Failed to load Settings. JSON parsing error at %s %d:%d %s",
                error.source,
                error.line,
                error.column,
                error.text);
        }
        return;
    }

    fromJson(rootJ);
    fclose(file);
    json_decref(rootJ);

    DINFO("[Prefabs] Loaded Settings from %s", path.c_str());
}
