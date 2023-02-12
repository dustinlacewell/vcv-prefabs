#pragma once

#include "client/StorageClient.hpp"

typedef std::function<std::vector<NewPatchInfo>(StorageClient, int)> QueryFunc;

struct QueryCache {
    std::string name;
    StorageClient client;
    QueryFunc queryFunc;

    std::vector<NewPatchInfo> cache;

    QueryCache(std::string name, StorageClient client);
    std::string getCacheFilename();
    std::string getPatchFilename(NewPatchInfo patchInfo, PatchFile fileInfo);
    int getLatestPatchId();

    json_t* readCacheFile();
    void writeCacheFile(json_t* rootJ);

    std::vector<NewPatchInfo> loadCache();
    void saveCache();

    std::vector<NewPatchInfo> getNewPatches();
    void downloadAll();
    void downloadFile(NewPatchInfo patchInfo, PatchFile fileInfo);
    void downloadPatch(NewPatchInfo patchInfo);
    void refresh();
};