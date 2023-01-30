#include "PrefabItem.hpp"

#include "ModuleItem.hpp"

ModuleItem::ModuleItem(Prefabs* module, std::string moduleName, PrefabSet prefabs)
{
    this->module = module;
    this->moduleName = moduleName;
    this->text = moduleName;
    this->rightText = RIGHT_ARROW;
    this->prefabs = prefabs;
}

Menu* ModuleItem::createChildMenu()
{
    auto menu = new Menu;
    for (auto prefab : prefabs) {
        auto item = new PrefabItem(module, prefab);
        menu->addChild(item);
    }
    return menu;
}