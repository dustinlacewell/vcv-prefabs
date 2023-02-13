#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct RackItem : ModularMenuItem {
    State* state;
    std::optional<Rack> rack;
    Tooltip* tooltip;
    std::string source;

    RackItem(State* module);
    RackItem(State* module, Rack rack);
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void step() override;
    void draw(const DrawArgs& args) override;
    void setRack(Rack rack);
    void unsetRack();
};