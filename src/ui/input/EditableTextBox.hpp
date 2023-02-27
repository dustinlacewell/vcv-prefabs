#pragma once

#include <rack.hpp>

using namespace rack;

#include "HoverableTextBox.hpp"

/*
* Basically a custom version of TextField.
* TextField::text holds the string being edited and HoverableTextBox::text
* holds the string being displayed. When the user stops editing (by pressing
* enter or clicking away), onDeselect() is called, which by default just sets
* the display text to the edited text. Override this to e.g. validate the text
* content.
*/
struct EditableTextBox : HoverableTextBox, TextField {

    bool isFocused = false;
    const static unsigned int defaultMaxTextLength = 6;
    unsigned int maxTextLength;

    EditableTextBox();

    void draw(const DrawArgs& args) override;

    void onButton(const event::Button& e) override;

    void onDragHover(const event::DragHover& e) override;

    void onHover(const event::Hover& e) override;

    void onHoverScroll(const event::HoverScroll& e) override;

    void onAction(const event::Action& e) override;

    void onSelectText(const event::SelectText& e) override;
    void onSelectKey(const event::SelectKey& e) override;

    void onSelect(const event::Select& e) override;
    void onDeselect(const event::Deselect& e) override;

    void step() override;
};