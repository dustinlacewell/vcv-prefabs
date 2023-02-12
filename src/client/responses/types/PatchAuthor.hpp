#pragma once

#include <rack.hpp>

struct PatchAuthor {
    int id;

    std::string slug;
    std::string name;

    json_t* toJson();

    void fromJson(json_t* rootJ);
};
