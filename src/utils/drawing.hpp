#pragma once

#include <rack.hpp>

using namespace rack;

void withPath(const Widget::DrawArgs& args, std::function<void()> f);
void withStroke(const Widget::DrawArgs& args, float size, NVGcolor color, std::function<void()> f);
void withFill(const Widget::DrawArgs& args, NVGcolor color, std::function<void()> f);
void withClip(const Widget::DrawArgs& args, math::Rect box, std::function<void()> f);

void withFont(const Widget::DrawArgs& args,
    std::string fontPath,
    float fontSize,
    std::function<void(std::shared_ptr<Font>)> f);
void withFont(const Widget::DrawArgs& args,
    std::string fontPath,
    float fontSize,
    int alignment,
    std::function<void(std::shared_ptr<Font>)> f);