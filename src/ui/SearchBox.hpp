#pragma once

#include <rack.hpp>
#include <regex>

using namespace rack;

struct SearchBox : ui::TextField {
    WeakPtr<ui::Menu> menu{nullptr};
    std::string lastText{};
    function<void(std::string)> onChange;
    std::vector<std::string> tokens;

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
        box.size.x = 0;

        text = rack::string::trim(text);

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