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
    localPrefabs->setCallback([](Rack rack) {
        std::unique_lock<std::mutex> lock(prefabsMutex);
        prefabsQueue.push(rack);
    });
    prefabSources.push_back(localPrefabs);

    // for each plugin add a prefab source
    for (auto plugin : rack::plugin::plugins) {
        auto pluginPrefabs = new FileSource(plugin->slug, asset::plugin(plugin, "res/selections"));
        pluginPrefabs->setCallback([](Rack rack) {
            std::unique_lock<std::mutex> lock(prefabsMutex);
            prefabsQueue.push(rack);
        });
        prefabSources.push_back(pluginPrefabs);
    }

    auto localPatches = new ArchiveSource("local", asset::user("patches"));
    localPatches->setCallback([](Rack rack) {
        std::unique_lock<std::mutex> lock(patchesMutex);
        patchesQueue.push(rack);
    });
    patchSources.push_back(localPatches);

    // for each plugin add a patch source
    for (auto plugin : rack::plugin::plugins) {
        auto pluginPatches = new ArchiveSource(plugin->slug, asset::plugin(plugin, "res/patches"));
        pluginPatches->setCallback([](Rack rack) {
            std::unique_lock<std::mutex> lock(patchesMutex);
            patchesQueue.push(rack);
            lock.unlock();
        });
        patchSources.push_back(pluginPatches);
    }
}

ThreadedStore::~ThreadedStore()
{
    for (auto source : prefabSources) {
        delete source;
    }
    for (auto source : patchSources) {
        delete source;
    }
}

void ThreadedStore::refresh()
{
    std::thread([this]() {
        refreshing++;
        for (auto source : prefabSources) {
            source->refresh();
        }
        for (auto source : patchSources) {
            source->refresh();
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