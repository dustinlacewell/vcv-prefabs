#pragma once

#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/models/State.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "ui/menus/scrolling/ScrollableItem.hpp"

struct RackItem : ModularItem {
    State* state;
    std::optional<Rack> rack;
    Tooltip* tooltip;
    std::string source;

    RackItem(State* module);
    RackItem(State* module, Rack rack);

    void onButton(const event::Button& e) override;
    void step() override;
    void setRack(Rack rack);
    void unsetRack();
    std::string getRightText() override;
    ScrollableMenu* createChildMenu() override;
    Widget* createTooltip();
    void createContextMenu();
};