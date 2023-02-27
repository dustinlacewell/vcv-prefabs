#pragma once

#include <rack.hpp>
#include "modules/prefabs/menus/main_menu/MainMenuBuilder.hpp"
#include "modules/prefabs/models/DiscoUpdater.hpp"
#include "modules/prefabs/models/ModuleIndex.hpp"
#include "modules/prefabs/models/State.hpp"
#include "ui/SearchBox.hpp"

using namespace rack;

struct IconWidget : OpaqueWidget {
    State* state;
    MainMenuBuilder* menuBuilder;
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