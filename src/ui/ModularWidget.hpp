#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularWidget : Widget {
    Widget* tooltip = nullptr;

    std::function<void()> stepCallback;
    std::function<Widget*()> tooltipCallback;
    std::function<bool()> visibleCallback;
    std::function<bool(const event::Button& e)> buttonCallback;
    std::function<bool(const event::Button& e)> rightClickCallback;

    std::function<void(const event::Hover& e)> hoverCallback;
    std::function<void(const event::Enter& e)> enterCallback;
    std::function<void(const event::Leave& e)> leaveCallback;

    std::function<void(const event::DragStart& e)> dragStartCallback;
    std::function<void(const event::DragEnd& e)> dragEndCallback;
    std::function<void(const DrawArgs& args)> drawCallback;

    ModularWidget();

    void setTooltip(Widget* tooltip);
    void step() override;
    void draw(const DrawArgs& args) override;
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void onDragStart(const event::DragStart& e) override;
    void onDragEnd(const event::DragEnd& e) override;
    void stepTooltip();
};