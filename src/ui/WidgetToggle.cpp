

#include "WidgetToggle.hpp"

WidgetToggle::WidgetToggle(Prefabs* module)
{
    this->module = module;
    this->color = nvgRGB(0xff, 0xd7, 0x14);

    if (module) {
        isLatched = [=]() {
            return module->widget && module->widget->state->showing;
        };
        latchCallback = [=]() {
            if (module->widget) {
                module->widget->state->showing = true;
                module->widget->state->reload();
            }
        };

        unlatchCallback = [=]() {
            if (module->widget) {
                module->widget->state->showing = false;
            }
        };
    }

    tooltipCallback = [=]() {
        auto tooltip = new Tooltip();
        tooltip->text = "Toggle widget / Refresh";
        return tooltip;
    };
}