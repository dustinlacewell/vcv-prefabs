#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularWidget : OpaqueWidget
{
    Widget* tooltip = nullptr;
    std::function<Widget*()> tooltipCallback;
    std::function<bool()> visibleCallback;
    std::function<bool(const event::Button& e)> buttonCallback;
    std::function<bool(const event::Button& e)> rightClickCallback;

    std::function<void(const event::DragStart& e)> dragStartCallback;
    std::function<void(const event::DragEnd& e)> dragEndCallback;

    ModularWidget();

    void setTooltip(Widget* tooltip);
    void step() override;
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void onDragStart(const event::DragStart& e) override;
    void onDragEnd(const event::DragEnd& e) override;
};