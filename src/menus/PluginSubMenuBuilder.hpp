#pragma once

#include <plugin.hpp>
#include <rack.hpp>
#include <tag.hpp>

#include "ModularMenuItem.hpp"
#include "ModularMenuLabel.hpp"
#include "PluginItem.hpp"
#include "PrefabItem.hpp"
#include "TagItem.hpp"
#include "models/ModuleIndex.hpp"
#include "ui/ModelBox.hpp"
#include "ui/SearchBox.hpp"
#include "ui/VerticalGroup.hpp"
#include "utils/keyboard.hpp"

using namespace rack;

struct PluginSubMenuData
{
    Plugin* plugin;
    std::vector<Model*> modules;
};

MenuItem* buildModuleSubItem(Model* model);
MenuItem* buildPluginSubMenu(PluginSubMenuData data);