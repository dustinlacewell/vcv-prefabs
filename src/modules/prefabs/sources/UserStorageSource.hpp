#pragma once

#include "ArchiveSource.hpp"

struct UserStorageSource : ArchiveSource {
    UserStorageSource(std::string slug, std::string root);
};