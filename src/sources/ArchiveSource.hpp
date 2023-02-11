#pragma once

#include "FileSource.hpp"

struct ArchiveSource : FileSource {
    ArchiveSource();
    ArchiveSource(std::string slug, std::string root);
    ~ArchiveSource();

    void load() override;

    json_t* readJson(std::string filePath) override;
    std::vector<std::string> filterFiles(std::vector<std::string>& files) override;
};