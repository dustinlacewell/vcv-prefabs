#include <rack.hpp>

#include "GroupItem.hpp"
#include "RackItem.hpp"

GroupItem::GroupItem(State* state, std::string groupName, std::set<Rack> groupRacks)
{
    this->state = state;
    this->group = groupName;
    this->text = groupName;
    this->racks = groupRacks;
    this->rightText = RIGHT_ARROW;
}

Menu* GroupItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto rack : racks) {
        auto item = new RackItem(state, rack);
        menu->addChild(item);
    }
    return menu;
}