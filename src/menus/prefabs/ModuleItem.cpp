#include "PrefabItem.hpp"

#include "ModuleItem.hpp"

ModuleItem::ModuleItem(State* state, std::string moduleName, PrefabSet prefabs)
{
    this->state = state;
    this->moduleName = moduleName;
    this->text = moduleName;
    this->rightText = RIGHT_ARROW;
    this->prefabs = prefabs;
}

Menu* ModuleItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto prefab : prefabs) {
        auto item = new PrefabItem(state, prefab);
        menu->addChild(item);
    }
    return menu;
}