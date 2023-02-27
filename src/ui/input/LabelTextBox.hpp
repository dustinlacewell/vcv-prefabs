#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/facade/Facade.hpp"
#include "ui/input/EditableTextBox.hpp"

struct LabelTextBox : EditableTextBox {
    Facade* module;

    std::function<void(std::string)> changeCallback;
    std::function<std::string()> textCallback;

    LabelTextBox(Facade* m);

    void onDeselect(const event::Deselect& e) override;

    void onSelectText(const event::SelectText& e) override;

    void onSelectKey(const event::SelectKey& e) override;

    void step() override;
};