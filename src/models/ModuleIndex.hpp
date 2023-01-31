#pragma once

#include <rack.hpp>
#include "Prefabs.hpp"

using namespace rack;

/**
* ModuleIndex is a helper service that accepts a search string
* It then breaks the string into tokens and then searches every module in every plugin
*/

struct ModuleIndex
{
    Prefabs* module;
    std::vector<Model*> results;

    ModuleIndex() : module(NULL){};
    ModuleIndex(Prefabs* module) : module(module){};
    void search(std::string searchString);
};