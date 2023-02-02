#include <dirent.h>
#include <sys/stat.h>
#include <rack.hpp>

#include "Prefabs.hpp"
#include "utils/logging.hpp"
#include "widgets/PrefabsWidget.hpp"

using namespace rack::dsp;

bool locked = false;

Prefabs::Prefabs()
{
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(STATUS_LIGHT, "Status light");

    widget = nullptr;
    float currentShow = params[SHOW_PARAM].getValue();
    lastShowParam = currentShow;
}

void Prefabs::process(const ProcessArgs& args)
{
    bool hasWidget = widget != nullptr;

    if (!hasWidget) {
        widget = findWidget();

        if (locked) {
            return;
        }

        if (widget == nullptr) {
            locked = true;
            widget = new IconWidget();
            APP->scene->addChildBelow(widget, APP->scene->browser);
            locked = false;
        }

        lastShowParam = widget->state->showing ? 1.0 : 0.0;
    }

    float currentShow = params[SHOW_PARAM].getValue();
    bool buttonClicked = abs(lastShowParam - currentShow) > 0.001;
    lastShowParam = currentShow;

    if (buttonClicked) {
        widget->state->showing = !(widget->state->showing);
    }

    lights[STATUS_LIGHT].setBrightness(widget->state->showing ? 1.0 : 0.0);
}

IconWidget* Prefabs::findWidget()
{
    if (widget) {
        return widget;
    }

    for (Widget* w : APP->scene->children) {

        IconWidget* foundWidget = dynamic_cast<IconWidget*>(w);

        if (!foundWidget) {
            continue;
        }

        return foundWidget;
    }

    return nullptr;
}

Model* modelPrefabs = createModel<Prefabs, PrefabsWidget>("Prefabs");
