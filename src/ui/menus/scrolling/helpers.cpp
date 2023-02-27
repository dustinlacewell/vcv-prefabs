#include "helpers.hpp"

ScrollableMenu* createScrollableMenu()
{
    ScrollableMenu* menu = new ScrollableMenu;
    menu->box.pos = APP->scene->mousePos;

    ui::MenuOverlay* menuOverlay = new ui::MenuOverlay;
    menuOverlay->addChild(menu);
    menuOverlay->bgColor = nvgRGBAf(0, 0, 0, 0.5);

    APP->scene->addChild(menuOverlay);

    return menu;
}

ModularMenuSeparator* createSeparator()
{
    auto w = new ModularMenuSeparator();
    w->box.size.x = 200;
    return w;
}