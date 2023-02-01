#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Prefab.hpp"
#include "PrefabSource.hpp"

struct PatchSource : PrefabSource
{
    PatchSource();
    PatchSource(Plugin* plugin);
    std::string rootPath() override;
    json_t* loadFile(std::string path) override;
};