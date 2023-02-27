#include "HoverableTextBox.hpp"

HoverableTextBox::HoverableTextBox() : LedLabel() {
    defaultColor = backgroundColor;
    hoverColor = nvgRGB(0xd8, 0xd8, 0xd8);
}

void HoverableTextBox::onHover(const event::Hover& e) {
    e.consume(this);  // to catch onEnter and onLeave
}

void HoverableTextBox::onEnter(const event::Enter& e) {
    state = BND_HOVER;
}
void HoverableTextBox::onLeave(const event::Leave& e) {
    state = BND_DEFAULT;
}

void HoverableTextBox::draw(const DrawArgs& args) {
    backgroundColor = state == BND_HOVER ? hoverColor : defaultColor;
    LedLabel::draw(args);
}
