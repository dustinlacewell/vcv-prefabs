#include "PatchResults.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/RackItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"
#include "utils/keyboard.hpp"

PatchResults::PatchResults(MenuState* menuState) {
    this->menuState = menuState;
    buildMenu();
}

void PatchResults::buildLabel() {
    auto sep = createSeparator();
    sep->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep);

    auto label = new ModularItem();
    label->visibleCallback = [this]() {
        return menuState->query != "";
    };
    label->text = "Patches";
    label->disabled = true;
    addChild(label);

    auto sep2 = createSeparator();
    sep2->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep2);
}

void PatchResults::buildResultsSubmenu() {
    auto ctrlWasPressed = modPressed(GLFW_MOD_CONTROL);

    auto subMenu = new ScrollableMenu();
    subMenu->maxHeight = 200;

    // create 15 local patch results
    for (int i = 0; i < 150; i++) {
        auto item = new RackItem(menuState->state);

        item->rightTextCallback = [this, i]() {
            if (i >= menuState->patchResults.results.size()) {
                return "";
            }

            auto& result = menuState->patchResults.results[i];

            if (result.metadata.has_value()) {
                return result.metadata->author.name.c_str();
            }

            return result.group.c_str();
        };

        item->visibleCallback = [this, item, i, ctrlWasPressed]() {
            // if i >= results.size, then return false
            if (i >= menuState->patchResults.results.size()) {
                item->unsetRack();
                return false;
            }

            // if search box is empty, return false
            if (menuState->query == "") {
                item->unsetRack();
                return false;
            }

            auto& result = menuState->patchResults.results[i];

            if (!result.isValid && !ctrlWasPressed) {
                item->unsetRack();
                return false;
            }

            item->setRack(result);
            item->rightText = result.group;

            if (result.metadata.has_value()) {
                auto author = result.metadata->author.name;
                item->rightText = result.isValid ? author : author + "!";
            }

            return true;
        };
        subMenu->addMenuItem(item);
    }
    addChild(subMenu);
}

void PatchResults::buildMenu() {
    buildLabel();
    buildResultsSubmenu();
}
