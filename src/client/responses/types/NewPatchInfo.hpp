#pragma once

#include <rack.hpp>

#include <chrono>
#include <rack.hpp>
#include "PatchAuthor.hpp"
#include "PatchFile.hpp"
#include "date/date.h"  // howard's date lib

using namespace date;
using namespace std::chrono;

struct NewPatchInfo {
    int id;

    std::string url;
    std::string slug;
    std::string title;
    std::string excerpt;
    std::string content;
    std::string revision;

    PatchAuthor author;

    std::string created;

    std::vector<PatchFile> files;

    NewPatchInfo()
    {
        id = 0;
        url = "";
        slug = "";
        title = "";
        excerpt = "";
        content = "";
        revision = "";
        author = PatchAuthor();
        created = "";
        files = std::vector<PatchFile>();
    }

    bool operator<(const NewPatchInfo& other) const;

    json_t* toJson();

    void fromJson(json_t* rootJ);
};