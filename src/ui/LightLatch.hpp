#pragma once

#include <rack.hpp>

#include "Latch.hpp"

struct LightLatch : Latch
{

    LightWidget* light;

    float brightness = 1.f;
    NVGcolor color = nvgRGB(0xff, 0xd7, 0x14);

    LightLatch();

    void step() override;
};