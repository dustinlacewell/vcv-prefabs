#include "PrefabResults.hpp"
#include "modules/prefabs/menus/main_menu/prefabs/RackItem.hpp"
#include "ui/menus/ModularMenuLabel.hpp"
#include "ui/menus/scrolling/helpers.hpp"

PrefabResults::PrefabResults(MenuState* menuState)
{
    this->menuState = menuState;
    buildMenu();
}

void PrefabResults::buildLabel()
{
    auto sep = createSeparator();
    sep->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep);

    auto label = new ModularMenuLabel();
    label->text = "Prefabs";
    label->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(label);

    auto sep2 = createSeparator();
    sep2->visibleCallback = [this]() {
        return menuState->query != "";
    };
    addChild(sep2);
}

void PrefabResults::buildResultsSubmenu()
{
    auto subMenu = new ScrollableMenu();
    subMenu->maxHeight = 200;

    for (const auto& sourcePair : menuState->state->prefabsIndex.sources) {
        auto slug = sourcePair.first;
        auto source = sourcePair.second;

        for (const auto& racks : source.racks) {
            auto item = new RackItem(menuState->state, racks);
            item->source = slug;
            item->rightTextCallback = [slug]() {
                return slug;
            };
            item->visibleCallback = [this, racks]() {
                bool nonEmpty = menuState->query != "";
                bool found = racks.name.find(menuState->query) != std::string::npos;
                return nonEmpty && found;
            };
            subMenu->addMenuItem(item);
        }
    }

    addChild(subMenu);
}

void PrefabResults::buildMenu()
{
    buildLabel();
    buildResultsSubmenu();
}
