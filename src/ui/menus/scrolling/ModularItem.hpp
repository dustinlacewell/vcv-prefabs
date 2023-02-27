#pragma once

#include <rack.hpp>
#include "ScrollableItem.hpp"

using namespace rack;

struct ModularItem : ScrollableItem {
    Widget* tooltip = nullptr;
    std::function<Widget*()> tooltipCallback;
    std::function<bool()> visibleCallback;
    std::function<std::string()> rightTextCallback;
    std::function<void(ModularItem*, ScrollableMenu*)> childMenuCallback;
    std::function<bool(const event::Button& e)> buttonCallback;
    std::function<bool(const event::Button& e)> rightClickCallback;

    ModularItem();
    ScrollableMenu* createChildMenu() override;

    void setTooltip(Widget* tooltip);
    void step() override;
    virtual std::string getRightText();
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void updateTooltip();
};