#pragma once

#include <rack.hpp>
#include "ModularWidget.hpp"

using namespace rack;

struct LedLabel : ModularWidget {
    std::string text;
    std::string fontPath = "res/fonts/RobotoMono-Bold.ttf";
    float fontSize;
    float letterSpacing;
    int alignment;
    Vec textOffset;
    NVGcolor defaultTextColor;
    NVGcolor textColor;  // This can be used to temporarily override text color
    NVGcolor backgroundColor;

    LedLabel();

    virtual void draw(const DrawArgs& args) override;
};
