#pragma once

#include <rack.hpp>

using namespace rack;

// enum for the different types of sorting
enum SortType
{
    ALPHA,
    USAGE,
    FAVORITE_THEN_ALPHA,
    FAVORITE_THEN_USAGE,
};

struct ModuleSorter
{
    SortType sortType;

    ModuleSorter();
    std::vector<Model*> sort(std::vector<Model*> modules);

    MenuItem* createSortTypeMenu();

    json_t* toJson();
    void fromJson(json_t* rootJ);
};
