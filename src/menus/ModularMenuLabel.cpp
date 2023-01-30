#include "ModularMenuLabel.hpp"

void ModularMenuLabel::step()
{
    if (visibleCallback) {
        setVisible(visibleCallback());
    }

    MenuLabel::step();
}