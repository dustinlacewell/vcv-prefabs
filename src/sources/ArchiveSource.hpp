#pragma once

#include "FileSource.hpp"

struct ArchiveSource : FileSource
{
    ArchiveSource();
    ArchiveSource(std::string slug, std::string root);
    ~ArchiveSource();

    json_t* readJson(std::string filePath) override;
};