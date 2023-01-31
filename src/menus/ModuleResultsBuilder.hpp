#pragma once

#include <rack.hpp>

#include "Prefabs.hpp"
#include "menus/PluginSubMenuBuilder.hpp"
#include "models/ModuleIndex.hpp"

using namespace rack;

MenuItem* buildModuleSearchResult(Prefabs* module, ModuleIndex* modules, int index);