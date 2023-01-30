#pragma once

#include <rack.hpp>
#include "Prefabs.hpp"
#include "menus/IconMenuBuilder.hpp"
#include "models/DiscoUpdater.hpp"
#include "models/ModuleIndex.hpp"
#include "ui/SearchBox.hpp"

using namespace rack;

struct IconWidget : OpaqueWidget
{
    Prefabs* module;

    IconMenuBuilder* menuBuilder;
    ModuleIndex modules;

    DiscoUpdater disco;

    IconWidget(Prefabs* module);
    void step() override;
    void draw(const DrawArgs& args) override;
    void onButton(const event::Button& e) override;
    void onDragMove(const event::DragMove& e) override;
    void createContextMenu();
};