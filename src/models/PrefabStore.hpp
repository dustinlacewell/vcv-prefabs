#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Prefab.hpp"
#include "PrefabSource.hpp"

struct PrefabStore
{
    int total = 0;
    std::map<std::string, PrefabSource> sources;

    PrefabStore();

    int getTotal();
    void refresh();
    void addSource(PrefabSource source);
    PrefabSource& getLocalSource();
};