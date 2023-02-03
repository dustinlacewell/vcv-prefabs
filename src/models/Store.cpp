#include <thread>

#include "Store.hpp"

Store::Store()
{
    prefabs = PrefabStore();
    patches = PatchStore();
}

void Store::refresh()
{
    if (locked) {
        return;
    }
    locked = true;
    std::thread([this]() {
        prefabs.refresh();
        patches.refresh();
        locked = false;
    }).detach();
}

bool Store::withLock(std::function<void(Store*)> callback)
{
    if (locked) {
        return false;
    }
    locked = true;
    callback(this);
    locked = false;
    return true;
}
