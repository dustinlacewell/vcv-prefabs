#pragma once

#include "ArchiveSource.hpp"

typedef std::tuple<std::string, int> StorageUser;
typedef std::tuple<std::string, int> StoragePatch;

struct StorageSource : ArchiveSource {
    std::string username;
    std::string password;
    std::vector<StorageUser> users;

    StorageSource(std::string username, std::string password, std::vector<StorageUser> users);
    StorageSource();
    ~StorageSource();

    void refresh() override;
    void refreshInThreads();
};