#pragma once

#include <rack.hpp>
#include "ModularWidget.hpp"

struct Latch : ModularWidget
{
    bool latched = false;

    std::function<void()> latchCallback;
    std::function<void()> unlatchCallback;
    std::function<bool()> isLatched;

    FramebufferWidget* fb;
    CircularShadow* shadow;
    SvgWidget* sw;
    std::vector<std::shared_ptr<window::Svg>> frames;

    Latch();
    ~Latch();

    void addFrame(std::shared_ptr<window::Svg> svg);
    virtual void step() override;
    virtual void onButton(const event::Button& e) override;
};