#pragma once

#include <rack.hpp>

using namespace rack;

struct Snowflake
{
    float x;
    float y;
    float dx;
    float dy;
};

struct SnowWidget : Widget
{
    float lastTime = 0.0;
    Vec firstOffset = Vec(0, 0);
    // This widget maintains a list of snowflakes
    // just small white circles
    // they flow down the screen randomly until they exit the box
    // then they are recycled

    // the snowflakes are stored in a vector
    std::vector<Snowflake> snowflakes;

    // the number of snowflakes to draw
    int numSnowflakes = 10000;

    // the maxSpeed of the snowflakes
    float fallSpeed = 35.0;
    float windSpeed = 15.0;

    // the size of the snowflakes
    float size = 2.0;

    // the color of the snowflakes
    NVGcolor color = nvgRGB(0xff, 0xff, 0xff);

    SnowWidget();
    void randomizeFlake(int i);
    void step() override;
    void draw(const DrawArgs& args) override;
};