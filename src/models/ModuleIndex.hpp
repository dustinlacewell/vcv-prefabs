#pragma once

#include <rack.hpp>

using namespace rack;

/**
* ModuleIndex is a helper service that accepts a search string
* It then breaks the string into tokens and then searches every module in every plugin
*/

struct ModuleIndex
{
    std::vector<Model*> results;

    ModuleIndex();
    void search(std::string searchString);
};