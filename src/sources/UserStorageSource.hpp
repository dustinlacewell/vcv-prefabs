#pragma once

#include "ArchiveSource.hpp"

typedef std::tuple<std::string, int> StorageUser;
typedef std::tuple<std::string, int> StoragePatch;

struct UserStorageSource : ArchiveSource {
    std::string username;
    std::string password;
    std::vector<StorageUser> users;

    UserStorageSource(std::string username, std::string password, std::vector<StorageUser> users);
    UserStorageSource();
    ~UserStorageSource();

    void load() override;
    void refreshInThreads();
    Rack* read(std::string filename) override;
};