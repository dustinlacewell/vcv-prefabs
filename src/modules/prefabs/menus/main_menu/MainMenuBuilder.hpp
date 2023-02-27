#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include "MenuState.hpp"
#include "modules/prefabs/menus/main_menu/layouts/StackedLayout.hpp"
#include "modules/prefabs/menus/main_menu/layouts/TabbedLayout.hpp"
#include "ui/SearchBox.hpp"
#include "ui/menus/scrolling/helpers.hpp"

using namespace rack;

struct MainMenuBuilder {
    MenuState menuState;
    SearchBox* searchBox;

    MainMenuBuilder(State* state) {
        menuState.state = state;
        menuState.moduleIndex = ModuleIndex(state);
        menuState.patchResults.candidateCallback = [this]() {
            auto state = this->menuState.state;
            auto racks = state->patchesIndex.racks;
            racks.insert(state->storageIndex.racks.begin(), state->storageIndex.racks.end());
            return racks;
        };
    }

    void createSearchBox() {
        searchBox = new SearchBox([this](std::string text) {
            menuState.moduleIndex.results.clear();

            menuState.patchResults.setQuery(text);

            menuState.query = text;

            if (text == "") {
                return;
            }

            menuState.moduleIndex.search(text);
        });
        searchBox->setExtents(Vec(250, BND_WIDGET_HEIGHT));
        menuState.rootMenu->addMenuItem(searchBox);
    }

    void build() {
        menuState.rootMenu = createScrollableMenu();
        menuState.moduleIndex.results.clear();
        menuState.query = "";

        createSearchBox();

        Widget* layout;

        if (menuState.state->layout == LayoutType::STACKED) {
            layout = new StackedLayout(&menuState);
        } else {
            layout = new TabbedLayout(&menuState);
        }

        menuState.rootMenu->addMenuItem(layout);
    }
};