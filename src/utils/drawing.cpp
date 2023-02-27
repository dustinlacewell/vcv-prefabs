#include "utils/drawing.hpp"

void withPath(const Widget::DrawArgs& args, std::function<void()> f) {
    nvgBeginPath(args.vg);
    f();
    nvgClosePath(args.vg);
}

void withStroke(const Widget::DrawArgs& args, float size, NVGcolor color, std::function<void()> f) {
    nvgStrokeWidth(args.vg, size);
    nvgStrokeColor(args.vg, color);
    f();
    nvgStroke(args.vg);
}

void withFill(const Widget::DrawArgs& args, NVGcolor color, std::function<void()> f) {
    nvgFillColor(args.vg, color);
    f();
    nvgFill(args.vg);
}

void withClip(const Widget::DrawArgs& args, math::Rect box, std::function<void()> f) {
    nvgScissor(args.vg, 0, 0, box.size.x, box.size.y);
    f();
    nvgResetScissor(args.vg);
}

void withFont(const Widget::DrawArgs& args,
    std::string fontPath,
    float fontSize,
    std::function<void(std::shared_ptr<Font>)> f) {
    auto font = APP->window->loadFont(fontPath);
    if (font && font->handle >= 0) {
        nvgFontFaceId(args.vg, font->handle);
        nvgFontSize(args.vg, fontSize);
        f(font);
    }
}

void withFont(const Widget::DrawArgs& args,
    std::string fontPath,
    float fontSize,
    int alignment,
    std::function<void(std::shared_ptr<Font>)> f) {
    auto font = APP->window->loadFont(fontPath);
    if (font && font->handle >= 0) {
        nvgFontFaceId(args.vg, font->handle);
        nvgFontSize(args.vg, fontSize);
        nvgTextAlign(args.vg, alignment);
        f(font);
    }
}