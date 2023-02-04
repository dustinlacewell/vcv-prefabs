#include "RackItem.hpp"

#include "ModuleItem.hpp"

ModuleItem::ModuleItem(State* state, std::string moduleName, RackSet racks) : racks(racks)
{
    this->state = state;
    this->moduleName = moduleName;
    this->text = moduleName;
    this->rightText = RIGHT_ARROW;
}

Menu* ModuleItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto rack : racks) {
        auto item = new RackItem(state, rack);
        menu->addChild(item);
    }
    return menu;
}