#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Prefab.hpp"
#include "PrefabSource.hpp"

struct PrefabStore
{
    std::map<std::string, PrefabSource> sources;

    PrefabStore();

    int total();
    void refresh();
    void addSource(PrefabSource source);
    PrefabSource& getLocalSource();
};