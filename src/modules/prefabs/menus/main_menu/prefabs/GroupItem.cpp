#include <rack.hpp>

#include "GroupItem.hpp"
#include "RackItem.hpp"

GroupItem::GroupItem(State* state, std::string groupName, std::set<Rack> groupRacks)
{
    this->state = state;
    this->group = groupName;
    this->text = groupName;
    this->racks = groupRacks;
    this->childMenuCallback = [racks = groupRacks, state](ModularItem* self, ScrollableMenu* menu) {
        for (auto rack : racks) {
            auto item = new RackItem(state, rack);
            menu->addMenuItem(item);
        }
        menu->box.size.x = 150;
    };
    box.size.y = 24;
    box.size.x = bndLabelWidth(APP->window->vg, -1, text.c_str());
}