#include "ThreadedStore.hpp"
#include <thread>
#include "FileSource.hpp"

std::mutex prefabsMutex;
std::mutex patchesMutex;

std::queue<Rack> prefabsQueue;
std::queue<Rack> patchesQueue;

int refreshing = 0;

ThreadedStore::ThreadedStore()
{
    auto localPrefabs = new FileSource("local", asset::user("selections"));
    addPrefabSource(localPrefabs);

    // for each plugin add a prefab source
    for (auto plugin : rack::plugin::plugins) {
        auto pluginSource = new FileSource(plugin->slug, asset::plugin(plugin, "res/selections"));
        addPrefabSource(pluginSource);
    }

    auto localPatches = new ArchiveSource("local", asset::user("patches"));
    addPatchSource(localPatches);

    // for each plugin add a patch source
    for (auto plugin : rack::plugin::plugins) {
        auto pluginPatches = new ArchiveSource(plugin->slug, asset::plugin(plugin, "res/patches"));
        addPatchSource(pluginPatches);
    }
}

void ThreadedStore::addPrefabSource(FileSource* source)
{
    source->setCallback([](Rack rack) {
        std::unique_lock<std::mutex> lock(prefabsMutex);
        prefabsQueue.push(rack);
    });
    prefabSources.push_back(source);
}

void ThreadedStore::addPatchSource(ArchiveSource* source)
{
    source->setCallback([](Rack rack) {
        std::unique_lock<std::mutex> lock(patchesMutex);
        patchesQueue.push(rack);
    });
    patchSources.push_back(source);
}

void ThreadedStore::addUserQueryCache(UserQueryCache* cache)
{
    userQueryCaches.push_back(cache);
}

ThreadedStore::~ThreadedStore()
{
    for (auto source : prefabSources) {
        delete source;
    }
    for (auto source : patchSources) {
        delete source;
    }
    for (auto cache : userQueryCaches) {
        delete cache;
    }
}

void ThreadedStore::load()
{
    std::thread([this]() {
        refreshing++;
        for (auto source : prefabSources) {
            source->load();
        }
        for (auto source : patchSources) {
            source->load();
        }
        refreshing--;
    }).detach();
}

void ThreadedStore::refresh()
{
    std::thread([this]() {
        refreshing++;
        for (auto cache : userQueryCaches) {
            cache->refresh();
        }
        refreshing--;
    }).detach();
}

int ThreadedStore::getTotalPrefabs()
{
    int total = 0;
    for (auto source : prefabSources) {
        total += source->getTotal();
    }
    return total;
}

int ThreadedStore::getTotalPatches()
{
    int total = 0;
    for (auto source : patchSources) {
        total += source->getTotal();
    }
    return total;
}