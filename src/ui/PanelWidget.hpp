#pragma once

#include "plugin.h"

#include "SvgHelper.hpp"
#include "ui/LedLabel.hpp"
#include "ui/LightLatch.hpp"

template <typename T>
struct PanelWidget : ModuleWidget {
    SvgHelper helper;

    PanelWidget(T* module, std::string filename) : helper(SvgHelper(asset::plugin(pluginInstance, filename))) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, filename)));
    }

    Tooltip* makeTooltip(std::string tooltipText) {
        auto tooltip = new Tooltip();
        tooltip->text = tooltipText;
        return tooltip;
    }

    void centerWidget(Widget* w, Vec center) {
        w->box.pos = Vec(center.x - w->box.size.x / 2, center.y - w->box.size.y / 2);
    };

    std::optional<LedLabel*> makeLabel(std::string name, std::string tooltipText) {
        auto position = helper.findNamed(name);

        if (!position.has_value()) {
            return std::nullopt;
        }

        auto label = new LedLabel();
        label->text = "0000";
        label->fontSize = 14;
        label->box.size = Vec(box.size.x, 20);
        label->tooltipCallback = [=]() {
            return makeTooltip(tooltipText);
        };

        centerWidget(label, mm2px(*position));
        addChild(label);
        return label;
    }

    std::optional<Widget*> makeLight(std::string name, std::string tooltipText, int lightId) {
        auto position = helper.findNamed(name);

        if (!position.has_value()) {
            return std::nullopt;
        }

        auto light = createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(7.5, 43)), module, lightId);
        centerWidget(light, mm2px(*position));
        addChild(light);

        return light;
    }

    std::optional<Widget*> makeToggle(std::string name,
        std::string tooltipText,
        std::function<bool()> stateCallback,
        std::function<void()> latchCallback,
        std::function<void()> unlatchCallback) {
        auto position = helper.findNamed(name);

        if (!position.has_value()) {
            return std::nullopt;
        }

        auto toggle = new LightLatch();
        toggle->tooltipCallback = [=]() {
            return makeTooltip(tooltipText);
        };
        toggle->isLatched = stateCallback;
        toggle->latchCallback = latchCallback;
        toggle->unlatchCallback = unlatchCallback;
        centerWidget(toggle, mm2px(*position));
        addChild(toggle);

        return toggle;
    }
};