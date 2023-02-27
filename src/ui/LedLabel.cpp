#include "LedLabel.hpp"
#include "utils/drawing.hpp"

LedLabel::LedLabel() {
    textColor = nvgRGB(0xff, 0xd7, 0x14);
    backgroundColor = nvgRGB(0xc8, 0xc8, 0xc8);
    fontSize = 12;
    fontPath = asset::system("res/fonts/ShareTechMono-Regular.ttf");
    letterSpacing = 0.f;
    textOffset = Vec(0, 0);
    alignment = NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE;
}

void LedLabel::draw(const DrawArgs& args) {
    ModularWidget::draw(args);

    withFont(args, fontPath, fontSize, alignment, [&](std::shared_ptr<Font> font) {
        withClip(args, box, [&] {
            nvgFillColor(args.vg, textColor);
            nvgTextLetterSpacing(args.vg, letterSpacing);
            nvgText(args.vg, box.size.x / 2 + textOffset.x, box.size.y / 2 + textOffset.y, text.c_str(), NULL);
        });
    });
}