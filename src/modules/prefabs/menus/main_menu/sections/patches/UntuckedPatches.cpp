#include "UntuckedPatches.hpp"
#include "LocalPatches.hpp"
#include "PluginPatches.hpp"
#include "StoragePatches.hpp"
#include "modules/prefabs/menus/main_menu/SectionHeader.hpp"
#include "ui/menus/ModularMenuLabel.hpp"

UntuckedPatches::UntuckedPatches(MenuState* menuState) {
    this->menuState = menuState;
    build();
}

void UntuckedPatches::build() {
    std::vector<Widget*> children = {
        new LocalPatches(menuState),
        new PluginPatches(menuState),
        new StoragePatches(menuState),
    };

    for (auto child : children) {
        addChild(child);
    }
}
