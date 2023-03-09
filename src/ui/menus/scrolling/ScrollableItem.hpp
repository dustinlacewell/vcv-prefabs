#pragma once

#include <rack.hpp>

#include "ScrollableMenu.hpp"

using namespace rack;

struct ScrollableItem : widget::OpaqueWidget {
    // default menu item color
    NVGcolor color = nvgRGB(0xff, 0xff, 0xff);
    std::string text;
    std::string rightText;
    bool disabled = false;

    ScrollableItem();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onEnter(const EnterEvent& e) override;
    void onDragDrop(const DragDropEvent& e) override;
    void doAction(bool consume = true);
    virtual ScrollableMenu* createChildMenu() { return NULL; }
    /** Override to handle behavior when user clicks the menu item.
	Event is consumed by default. Unconsume to prevent the menu from being closed.
	If Ctrl (Cmd on Mac) is held, the event is *not* pre-consumed, so if your menu must be closed, always consume the event.
	*/
    void onAction(const ActionEvent& e) override;
};