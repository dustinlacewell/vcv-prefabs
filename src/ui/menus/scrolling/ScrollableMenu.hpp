#pragma once

#include <optional>
#include <rack.hpp>

using namespace rack;

struct ScrollableMenu : ScrollWidget {
    List* list = NULL;
    std::optional<float> maxHeight;

    ScrollableMenu* parentMenu = NULL;
    ScrollableMenu* childMenu = NULL;
    /** The entry which created the child menu */
    Widget* activeEntry = NULL;
    BNDcornerFlags cornerFlags = BND_CORNER_NONE;

    ScrollableMenu();
    ~ScrollableMenu();
    void addMenuItem(Widget* child);
    void setChildMenu(ScrollableMenu* menu);
    void step() override;
    void draw(const DrawArgs& args) override;
    bool isEmbedded();
};
