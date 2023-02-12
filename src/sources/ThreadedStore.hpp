#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

#include <plugin.hpp>
#include <rack.hpp>
#include "ArchiveSource.hpp"
#include "UserQueryCache.hpp"
#include "UserStorageSource.hpp"

using namespace rack;

extern std::mutex prefabsMutex;
extern std::mutex patchesMutex;
extern std::mutex storageMutex;

extern std::queue<Rack> prefabsQueue;
extern std::queue<Rack> patchesQueue;
extern std::queue<Rack> storageQueue;

extern int refreshing;

struct ThreadedStore {
    std::vector<FileSource*> prefabSources;
    std::vector<ArchiveSource*> patchSources;
    std::vector<UserStorageSource*> userStorageSources;

    std::vector<UserQueryCache*> userQueryCaches;

    int getTotalPrefabs();
    int getTotalPatches();

    ThreadedStore();
    ~ThreadedStore();
    void load();

    void addPrefabSource(FileSource* source);
    void addPatchSource(ArchiveSource* source);
    void addStorageSource(UserStorageSource* source);
    void addUserQueryCache(UserQueryCache* cache);
    void refresh();
};