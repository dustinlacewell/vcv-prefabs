#include "ModularMenuSeparator.hpp"

void ModularMenuSeparator::step()
{
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    MenuSeparator::step();
}