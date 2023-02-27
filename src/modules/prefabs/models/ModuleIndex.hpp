#pragma once

#include <rack.hpp>
#include "State.hpp"

using namespace rack;

/**
* ModuleIndex is a helper service that accepts a search string
* It then breaks the string into tokens and then searches every state in every plugin
*/

struct ModuleIndex
{
    State* state;
    std::vector<Model*> results;

    ModuleIndex() : state(NULL){};
    ModuleIndex(State* state) : state(state){};
    void search(std::string searchString);
};