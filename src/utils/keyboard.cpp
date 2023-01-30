#include "keyboard.hpp"

bool modPressed(int flags)
{
    return (APP->window->getMods() & RACK_MOD_MASK) == flags;
}