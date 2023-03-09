#include "ModularItem.hpp"

ModularItem::ModularItem() {
    childMenuCallback = nullptr;
}

ModularItem::~ModularItem() {
    if (tooltip) {
        tooltip->requestDelete();
    }
}

ScrollableMenu* ModularItem::createChildMenu() {
    ScrollableMenu* menu = new ScrollableMenu;

    if (!childMenuCallback) {
        return menu;
    }

    childMenuCallback(this, menu);
    rightText = RIGHT_ARROW;
    return menu;
}

std::string ModularItem::getRightText() {
    std::string text = "";

    if (rightTextCallback) {
        text = rightTextCallback();
    }

    if (childMenuCallback) {
        text = text + RIGHT_ARROW;
    }

    return text;
}

void ModularItem::updateTooltip() {
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
        tooltip->box = tooltip->box.nudge(overlay->box.zeroPos());
    }
}

void ModularItem::step() {
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    rightText = getRightText();

    updateTooltip();

    ScrollableItem::step();
}

void ModularItem::onButton(const event::Button& e) {
    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        if (this->buttonCallback) {
            if (this->buttonCallback(e)) {
                // Close menu
                MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
                if (overlay) {
                    overlay->requestDelete();
                }

                if (e.isConsumed() && childMenuCallback) {
                    e.unconsume();
                }
            }
        }
    } else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (this->rightClickCallback) {
            if (this->rightClickCallback(e)) {
                // Close menu
                MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
                if (overlay) {
                    overlay->requestDelete();
                }

                if (e.isConsumed() && childMenuCallback) {
                    e.unconsume();
                }
            }
        }
    }
}

void ModularItem::setTooltip(Widget* tooltip) {
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

void ModularItem::onHover(const event::Hover& e) {
    e.consume(this);

    ScrollableItem::onHover(e);
}

void ModularItem::onEnter(const event::Enter& e) {
    if (!tooltip && tooltipCallback) {
        setTooltip(tooltipCallback());
    }

    if (tooltip) {
        tooltip->show();
    }

    ScrollableItem::onEnter(e);
}

void ModularItem::onLeave(const event::Leave& e) {
    if (tooltip) {
        tooltip->hide();
    }

    ScrollableItem::onLeave(e);
}
