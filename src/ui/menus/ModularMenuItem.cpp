#include "ModularMenuItem.hpp"

ModularMenuItem::ModularMenuItem()
{
    childMenuCallback = nullptr;
}

Menu* ModularMenuItem::createChildMenu()
{
    Menu* menu = new Menu;

    if (!childMenuCallback) {
        return menu;
    }

    childMenuCallback(this, menu);
    rightText = RIGHT_ARROW;
    return menu;
}

std::string ModularMenuItem::getRightText()
{
    std::string text = "";

    if (rightTextCallback) {
        text = rightTextCallback();
    }

    if (childMenuCallback) {
        text = text + RIGHT_ARROW;
    }

    return text;
}

void ModularMenuItem::step()
{
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    rightText = getRightText();

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

    MenuItem::step();
}

void ModularMenuItem::onButton(const event::Button& e)
{
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
    }
    else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (this->rightClickCallback) {
            this->rightClickCallback(e);
        }
    }
}

void ModularMenuItem::setTooltip(Widget* tooltip)
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

void ModularMenuItem::onHover(const event::Hover& e)
{
    if (tooltip || tooltipCallback) {
        e.consume(this);
    }

    MenuItem::onHover(e);
}

void ModularMenuItem::onEnter(const event::Enter& e)
{
    if (!tooltip && tooltipCallback) {
        setTooltip(tooltipCallback());
    }

    if (tooltip) {
        tooltip->show();
    }

    MenuItem::onEnter(e);
}

void ModularMenuItem::onLeave(const event::Leave& e)
{
    if (tooltip) {
        tooltip->hide();
    }

    MenuItem::onLeave(e);
}
