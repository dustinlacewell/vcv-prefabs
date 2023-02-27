#include "UntuckedPrefabs.hpp"
#include "LocalPrefabs.hpp"
#include "PluginPrefabs.hpp"
#include "ui/menus/ModularMenuLabel.hpp"

UntuckedPrefabs::UntuckedPrefabs(MenuState* menuState) {
    this->menuState = menuState;
    build();
}

void UntuckedPrefabs::build() {
    std::vector<Widget*> children = {
        new LocalPrefabs(menuState),
        new PluginPrefabs(menuState),
    };

    for (auto child : children) {
        addChild(child);
    }
}
