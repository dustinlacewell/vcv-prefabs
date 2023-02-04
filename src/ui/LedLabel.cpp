#include "LedLabel.hpp"

void LedLabel::draw(const DrawArgs& args)
{
    // draw the text
    auto fontPath = asset::system("res/fonts/ShareTechMono-Regular.ttf");
    std::shared_ptr<window::Font> font = APP->window->loadFont(fontPath);
    if (font && font->handle >= 0) {
        nvgFillColor(args.vg, color);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        // text is positioned from the bottom left
        // center it vertically in the box.size.y by it's height
        float textHeight = fontSize * 1.2;
        float textY = (box.size.y - textHeight) / 2.0;
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFontSize(args.vg, fontSize);
        nvgText(args.vg, box.size.x / 2, box.size.y / 2, text.c_str(), NULL);
    }
}