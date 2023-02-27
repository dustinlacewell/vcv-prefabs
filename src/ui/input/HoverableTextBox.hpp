#pragma once

#include <rack.hpp>

using namespace rack;

#include "ui/LedLabel.hpp"

// a LedLabel that changes its background color when hovered
struct HoverableTextBox : LedLabel {
    BNDwidgetState state = BND_DEFAULT;
    NVGcolor defaultColor;
    NVGcolor hoverColor;

    HoverableTextBox();

    void onHover(const event::Hover& e) override;

    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;

    void draw(const DrawArgs& args) override;
};