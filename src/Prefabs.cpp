#include <rack.hpp>

#include "utils/logging.hpp"
#include "widgets/FakeBrowser.hpp"
#include "widgets/PrefabsWidget.hpp"

using namespace rack::dsp;

bool locked = false;

Prefabs::Prefabs()
{
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(WIDGET_LIGHT, "Status light");

    widget = nullptr;

    process(ProcessArgs());
}

void Prefabs::enableBrowserMode()
{
    if (widget && widget->state->browserMode) {
        auto browser = dynamic_cast<FakeBrowser*>(APP->scene->browser);
        if (!browser) {
            new FakeBrowser();
        }
    }
}

void Prefabs::disableBrowserMode()
{
    if (widget && !widget->state->browserMode) {
        auto browser = dynamic_cast<FakeBrowser*>(APP->scene->browser);
        if (browser) {
            browser->disable();
        }
    }
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
            widget->state->refresh();
            APP->scene->addChildBelow(widget, APP->scene->browser);
            enableBrowserMode();
            locked = false;
        }
    }

    lights[WIDGET_LIGHT].setBrightness(widget->state->showing ? 1.0 : 0.0);
    lights[REFRESH_LIGHT].setBrightness(refreshing ? 1.0 : 0.0);
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
