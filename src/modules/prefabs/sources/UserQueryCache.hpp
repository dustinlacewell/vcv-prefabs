#pragma once

#include <rack.hpp>

#include "QueryCache.hpp"

struct UserQueryCache : QueryCache {
    int userId;
    std::string userSlug;

    UserQueryCache(std::string userSlug, int userId, StorageClient client);
};