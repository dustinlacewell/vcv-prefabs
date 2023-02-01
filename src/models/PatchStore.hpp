#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "PatchSource.hpp"
#include "Prefab.hpp"
#include "PrefabSource.hpp"

struct PatchStore
{
    std::map<std::string, PatchSource> sources;

    PatchStore();

    int total();
    void refresh();
    void addSource(PatchSource source);
    PatchSource& getLocalSource();
};