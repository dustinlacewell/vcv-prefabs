
#include "LightLatch.hpp"

LightLatch::LightLatch()
{
    light = new LightWidget();
    light->box.pos = Vec(0, 0);
    light->box.size = Vec(5, 5);
    light->color = color;
    addChild(light);
}

void LightLatch::step()
{
    Latch::step();
    light->color = latched ? color : nvgRGBAf(0, 0, 0, 0);

    // center light in box
    auto center = box.size / 2.0;
    light->box.pos = center - light->box.size / 2.0;
}