#include "ModularWidget.hpp"

ModularWidget::ModularWidget() : OpaqueWidget() {}

void ModularWidget::step()
{
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    if (tooltip) {
        auto pos = APP->scene->mousePos.plus(math::Vec(10, 10));

        // make sure tooltip is on screen
        auto windowSize = APP->window->getSize();
        auto size = tooltip->box.size;

        if (pos.x + size.x > windowSize.x) {
            pos.x = windowSize.x - size.x;
        }

        if (pos.y + size.y > windowSize.y) {
            pos.y = windowSize.y - size.y;
        }

        tooltip->box.pos = pos;
    }

    Widget::step();
}

void ModularWidget::onButton(const event::Button& e)
{
    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        if (this->buttonCallback) {
            this->buttonCallback(e);
        }
    }
    else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (this->rightClickCallback) {
            this->rightClickCallback(e);
        }
    }
}

void ModularWidget::setTooltip(Widget* tooltip)
{
    if (this->tooltip) {
        this->tooltip->requestDelete();
        this->tooltip = nullptr;
    }

    if (tooltip) {
        APP->scene->addChild(tooltip);
        this->tooltip = tooltip;
        tooltip->hide();
    }
}

void ModularWidget::onHover(const event::Hover& e)
{
    if (tooltip || tooltipCallback) {
        e.consume(this);
    }

    Widget::onHover(e);
}

void ModularWidget::onEnter(const event::Enter& e)
{
    if (!tooltip && tooltipCallback) {
        setTooltip(tooltipCallback());
    }

    if (tooltip) {
        tooltip->show();
    }

    Widget::onEnter(e);
}

void ModularWidget::onLeave(const event::Leave& e)
{
    if (tooltip) {
        tooltip->hide();
    }

    Widget::onLeave(e);
}

void ModularWidget::onDragStart(const event::DragStart& e)
{
    if (dragStartCallback) {
        dragStartCallback(e);
    }

    Widget::onDragStart(e);
}

void ModularWidget::onDragEnd(const event::DragEnd& e)
{
    if (dragEndCallback) {
        dragEndCallback(e);
    }

    Widget::onDragEnd(e);
}
