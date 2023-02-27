#include "LabelTextBox.hpp"

LabelTextBox::LabelTextBox(Facade* m) : EditableTextBox() {
    module = m;
}

void LabelTextBox::onDeselect(const event::Deselect& e) {
    isFocused = false;
    e.consume(NULL);
}

void LabelTextBox::onSelectText(const event::SelectText& e) {
    auto oldText = TextField::text;
    EditableTextBox::onSelectText(e);
    if (oldText != TextField::text) {
        if (changeCallback) {
            changeCallback(TextField::text);
        }
    }
}

void LabelTextBox::onSelectKey(const event::SelectKey& e) {
    auto oldText = TextField::text;
    EditableTextBox::onSelectKey(e);
    if (oldText != TextField::text) {
        if (changeCallback) {
            changeCallback(TextField::text);
        }
    }
}

void LabelTextBox::step() {
    EditableTextBox::step();
    if (!module)
        return;

    if (textCallback) {
        std::string text = textCallback();
        HoverableTextBox::text = text;
        if (!isFocused) {
            TextField::setText(text);
        }
    }
}