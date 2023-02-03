#pragma once

#include <rack.hpp>
#include "PatchStore.hpp"
#include "PrefabStore.hpp"

using namespace rack;

struct Store
{

    bool locked = false;

    PrefabStore prefabs;
    PatchStore patches;

    Store();
    void refresh();
    bool withLock(std::function<void(Store*)> callback);
};