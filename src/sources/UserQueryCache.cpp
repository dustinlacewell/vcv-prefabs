#include "UserQueryCache.hpp"

UserQueryCache::UserQueryCache(std::string userSlug, int userId, StorageClient client) : QueryCache(userSlug, client)
{
    this->userSlug = userSlug;
    this->userId = userId;
    this->queryFunc = [this](StorageClient client, int count) {
        QINFO("Fetching user patches for user %s with offset %d", this->userSlug.c_str(), count);
        auto patches = client.fetchUserPatches(this->userId, 0);
        std::vector<NewPatchInfo> patchInfos;

        for (auto patch : patches) {
            // try to find patch with same id in this->cache
            auto it = std::find_if(this->cache.begin(), this->cache.end(), [patch](NewPatchInfo patchInfo) {
                return patchInfo.id == patch.id;
            });
            // if found skip
            if (it != this->cache.end()) {
                continue;
            }
            auto patchInfo = client.fetchPatchInfo(patch.id);
            if (patchInfo.has_value()) {
                patchInfos.push_back(patchInfo.value());
            }
        }

        return patchInfos;
    };
}