#include <rack.hpp>

#include "PrefabItem.hpp"
#include "TagItem.hpp"

TagItem::TagItem(State* state, std::string tag, std::set<Prefab> prefabs)
{
    this->state = state;
    this->tag = tag;
    this->text = tag;
    this->prefabs = prefabs;
    this->rightText = RIGHT_ARROW;
}

Menu* TagItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto prefab : prefabs) {
        auto item = new PrefabItem(state, prefab);
        menu->addChild(item);
    }
    return menu;
}