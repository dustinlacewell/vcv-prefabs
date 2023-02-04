#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

#include <plugin.hpp>
#include <rack.hpp>
#include "ArchiveSource.hpp"

using namespace rack;

extern std::mutex prefabsMutex;
extern std::mutex patchesMutex;

extern std::queue<Rack> prefabsQueue;
extern std::queue<Rack> patchesQueue;

extern int refreshing;

struct ThreadedStore
{
    std::vector<FileSource*> prefabSources;
    std::vector<ArchiveSource*> patchSources;

    int getTotalPrefabs();
    int getTotalPatches();

    ThreadedStore();
    ~ThreadedStore();
    void refresh();
};