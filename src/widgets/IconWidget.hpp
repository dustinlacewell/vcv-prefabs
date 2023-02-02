#pragma once

#include <rack.hpp>
#include "menus/IconMenuBuilder.hpp"
#include "models/DiscoUpdater.hpp"
#include "models/ModuleIndex.hpp"
#include "models/State.hpp"
#include "ui/SearchBox.hpp"

using namespace rack;

struct IconWidget : OpaqueWidget
{
    State* state;
    IconMenuBuilder* menuBuilder;
    ModuleIndex modules;
    DiscoUpdater disco;

    IconWidget();
    ~IconWidget();
    void step() override;
    void draw(const DrawArgs& args) override;
    void onButton(const event::Button& e) override;
    void onDragMove(const event::DragMove& e) override;
    void onDragEnd(const event::DragEnd& e) override;
    void createContextMenu();
};