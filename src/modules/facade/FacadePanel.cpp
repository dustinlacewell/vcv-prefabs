// to avoid undefined reference to `glGenTextures'

#include <rack.hpp>
#include <ui/Slider.hpp>

#include "Facade.hpp"
#include "FacadePanel.hpp"

#include "SvgHelper.hpp"
#include "plugin.h"
#include "ui/input/LabelTextBox.hpp"

FacadePanel::FacadePanel(Facade* module) : PanelWidget<Facade>(module, "res/Facade.svg") {
    auto helper = SvgHelper(asset::plugin(pluginInstance, "res/Facade.svg"));

    helper.forEachPrefixed("label_", [&](int i, Vec pos) {
        auto input = new LabelTextBox(module);
        ((HoverableTextBox*)input)->box.size = mm2px(Vec(12, 4.397));
        if (module) {
            input->changeCallback = [module, i](std::string text) {
                module->labels[i] = text;
            };
            input->textCallback = [module, i]() {
                return module->labels[i];
            };
        }

        centerWidget((LedLabel*)input, mm2px(pos));
        addChild((HoverableTextBox*)input);
    });

    helper.forEachMatched("in_(\\d+)", [&](std::vector<std::string> captures, Vec pos) {
        auto id = std::stoi(std::string(captures[0]));
        auto cvInput1 = new SvgPort();
        auto svg1 = Svg::load(asset::plugin(pluginInstance, "res/InPort.svg"));
        cvInput1->setSvg(svg1);
        cvInput1->portId = Facade::IN_1 + id - 1;
        cvInput1->module = module;
        cvInput1->type = Port::INPUT;
        centerWidget(cvInput1, mm2px(pos));
        addChild(cvInput1);
    });

    helper.forEachMatched("out_(\\d+)", [&](std::vector<std::string> captures, Vec pos) {
        auto id = std::stoi(captures[0]);
        auto cvOutput1 = new SvgPort();
        auto svg1 = Svg::load(asset::plugin(pluginInstance, "res/OutPort.svg"));
        cvOutput1->setSvg(svg1);
        cvOutput1->portId = Facade::OUT_1 + id - 1;
        cvOutput1->module = module;
        cvOutput1->type = Port::OUTPUT;
        centerWidget(cvOutput1, mm2px(pos));
        addChild(cvOutput1);
    });
}