#pragma once

#include <rack.hpp>
#include "ModularWidget.hpp"

using namespace rack;

struct LedLabel : ModularWidget
{
    int fontSize = 12;
    std::string text;
    NVGcolor color = nvgRGB(0xff, 0xd7, 0x14);

    void draw(const DrawArgs& args) override;
};