#include "LedLabel.hpp"

void LedLabel::draw(const DrawArgs& args)
{
    auto fontPath = asset::system("res/fonts/ShareTechMono-Regular.ttf");
    std::shared_ptr<window::Font> font = APP->window->loadFont(fontPath);
    if (font && font->handle >= 0) {
        nvgFillColor(args.vg, color);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, fontSize);
        nvgText(args.vg, 0, 0, text.c_str(), NULL);
    }
}