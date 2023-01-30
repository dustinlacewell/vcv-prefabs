#pragma once

#include <rack.hpp>

// a function that interpolates between the colors of the rainbow
// takes a value between 0 and 1 and returns a NVGcolor
NVGcolor rainbow(float t)
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
    if (t < 0.25f) {
        r = 0.0f;
        g = 4.0f * t;
        b = 1.0f;
    }
    else if (t < 0.5f) {
        r = 0.0f;
        g = 1.0f;
        b = 1.0f + 4.0f * (0.25f - t);
    }
    else if (t < 0.75f) {
        r = 4.0f * (t - 0.5f);
        g = 1.0f;
        b = 0.0f;
    }
    else {
        r = 1.0f;
        g = 1.0f + 4.0f * (0.75f - t);
        b = 0.0f;
    }
    return nvgRGBA(r * 255, g * 255, b * 255, a * 255);
}