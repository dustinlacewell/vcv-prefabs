#pragma once

#include <rack.hpp>

using namespace rack;

#include "menus/ModularMenuItem.hpp"
#include "models/State.hpp"

struct PrefabItem : ModularMenuItem
{
    State* state;
    Prefab prefab;
    Tooltip* tooltip;
    std::string source;

    PrefabItem(State* module, Prefab prefab);
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void step() override;
    void draw(const DrawArgs& args) override;
};