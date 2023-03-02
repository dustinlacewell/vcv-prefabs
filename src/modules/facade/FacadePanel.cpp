// to avoid undefined reference to `glGenTextures'

#include <rack.hpp>
#include <ui/Slider.hpp>

#include "Facade.hpp"
#include "FacadePanel.hpp"

#include "plugin.h"
#include "ui/input/LabelTextBox.hpp"

FacadePanel::FacadePanel(Facade* module)
    : PanelWidget<Facade>(module, asset::plugin(pluginInstance, "res/Facade.svg")) {
    forEachPrefixed("label_", [&](int i, Vec pos) {
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

        auto halfBox = ((HoverableTextBox*)input)->box.size.div(2);
        centerWidget((LedLabel*)input, pos);
        addChild((HoverableTextBox*)input);
    });

    forEachMatched("in_(\\d+)", [&](std::vector<std::string> captures, Vec pos) {
        auto id = std::stoi(std::string(captures[0]));
        auto cvInput1 = new SvgPort();
        auto svg1 = Svg::load(asset::plugin(pluginInstance, "res/InPort.svg"));
        cvInput1->setSvg(svg1);
        cvInput1->portId = Facade::IN_1 + id - 1;
        cvInput1->module = module;
        cvInput1->type = Port::INPUT;
        centerWidget(cvInput1, pos);
        addChild(cvInput1);
    });

    forEachMatched("out_(\\d+)", [&](std::vector<std::string> captures, Vec pos) {
        auto id = std::stoi(captures[0]);
        auto cvOutput1 = new SvgPort();
        auto svg1 = Svg::load(asset::plugin(pluginInstance, "res/OutPort.svg"));
        cvOutput1->setSvg(svg1);
        cvOutput1->portId = Facade::OUT_1 + id - 1;
        cvOutput1->module = module;
        cvOutput1->type = Port::OUTPUT;
        centerWidget(cvOutput1, pos);
        addChild(cvOutput1);
    });
}