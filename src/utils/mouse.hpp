#pragma once

#include <rack.hpp>

using namespace rack;

bool isLeftClick(const event::Button& e)
{
    return e.button == GLFW_MOUSE_BUTTON_LEFT && e.action == GLFW_PRESS;
}

bool isRightClick(const event::Button& e)
{
    return e.button == GLFW_MOUSE_BUTTON_RIGHT && e.action == GLFW_PRESS;
}