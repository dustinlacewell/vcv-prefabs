#pragma once

#include <rack.hpp>

using namespace rack;

#include "Prefabs.hpp"
#include "menus/ModularMenuItem.hpp"

struct PrefabItem : ModularMenuItem
{
    Prefabs* module;
    Prefab prefab;
    Tooltip* tooltip;

    PrefabItem(Prefabs* module, Prefab prefab);
    void onButton(const event::Button& e) override;
    void onHover(const event::Hover& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void step() override;
    void draw(const DrawArgs& args) override;
};