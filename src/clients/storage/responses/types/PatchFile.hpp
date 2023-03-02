#pragma once

#include <rack.hpp>

struct PatchFile {
    int id;

    std::string url;
    int filesize;
    std::string filename;

    json_t* toJson();

    void fromJson(json_t* rootJ);
};