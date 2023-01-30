#pragma once

#include <rack.hpp>

using namespace rack;

struct SearchBox : ui::TextField
{
    WeakPtr<ui::Menu> menu{nullptr};
    std::string lastText{};
    function<void(std::string)> onChange;

    SearchBox(function<void(std::string)> onChange)
    {
        this->onChange = onChange;
        text = "";
        lastText = text;
        multiline = false;
        placeholder = "Search";
    }

    void step() override
    {
        if (text != lastText) {
            onChange(text);
        }
        lastText = text;
        // Keep selected
        APP->event->setSelectedWidget(this);

        TextField::step();
    }

    void draw(const DrawArgs& args) override { TextField::draw(args); }

    void setPos(Vec pos) { TextField::box.pos = pos; }

    void setExtents(Vec size) { TextField::box.size = size; }
};