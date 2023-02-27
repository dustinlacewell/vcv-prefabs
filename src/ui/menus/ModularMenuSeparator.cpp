#include "ModularMenuSeparator.hpp"

void ModularMenuSeparator::step()
{
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    box.size.x = 0;

    MenuSeparator::step();
}