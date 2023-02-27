#include "ModularWidget.hpp"

ModularWidget::ModularWidget() : Widget() {}

void ModularWidget::stepTooltip() {
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
        auto overlay = this->getAncestorOfType<MenuOverlay>();
        if (overlay) {
            tooltip->box = tooltip->box.nudge(overlay->box.zeroPos());
        }
    }
}

void ModularWidget::step() {
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    if (stepCallback) {
        stepCallback();
    }

    stepTooltip();

    Widget::step();
}

void ModularWidget::onButton(const event::Button& e) {
    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        if (this->buttonCallback) {
            this->buttonCallback(e);
            return;
        }
    } else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (this->rightClickCallback) {
            this->rightClickCallback(e);
            return;
        }
    }

    Widget::onButton(e);
}

void ModularWidget::setTooltip(Widget* tooltip) {
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

void ModularWidget::onHover(const event::Hover& e) {
    Widget::onHover(e);

    if (tooltip || tooltipCallback) {
        e.consume(this);
    }

    if (hoverCallback) {
        hoverCallback(e);
    }

    if (enterCallback || leaveCallback) {
        e.consume(this);
    }
}

void ModularWidget::onEnter(const event::Enter& e) {
    if (!tooltip && tooltipCallback) {
        auto tooltip = tooltipCallback();
        setTooltip(tooltip);
    }

    if (tooltip) {
        tooltip->show();
    }

    if (enterCallback) {
        enterCallback(e);
    }

    Widget::onEnter(e);
}

void ModularWidget::onLeave(const event::Leave& e) {
    if (tooltip) {
        tooltip->hide();
    }

    if (leaveCallback) {
        leaveCallback(e);
    }

    Widget::onLeave(e);
}

void ModularWidget::onDragStart(const event::DragStart& e) {
    if (dragStartCallback) {
        dragStartCallback(e);
    }

    Widget::onDragStart(e);
}

void ModularWidget::onDragEnd(const event::DragEnd& e) {
    if (dragEndCallback) {
        dragEndCallback(e);
    }

    Widget::onDragEnd(e);
}