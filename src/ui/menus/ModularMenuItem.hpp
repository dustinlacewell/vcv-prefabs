#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularMenuItem : MenuItem {
    Widget* tooltip = nullptr;
    std::function<Widget*()> tooltipCallback;
    std::function<bool()> visibleCallback;
    std::function<std::string()> rightTextCallback;
    std::function<void(ModularMenuItem*, Menu*)> childMenuCallback;
    std::function<bool(const event::Button& e)> buttonCallback;
    std::function<bool(const event::Button& e)> rightClickCallback;

    ModularMenuItem();
    Menu* createChildMenu() override;

    void setTooltip(Widget* tooltip);
    void step() override;
    virtual std::string getRightText();
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
};