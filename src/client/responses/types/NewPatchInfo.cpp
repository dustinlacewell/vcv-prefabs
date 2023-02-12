#include "NewPatchInfo.hpp"

bool NewPatchInfo::operator<(const NewPatchInfo& other) const
{
    std::chrono::system_clock::time_point tp1, tp2;
    std::istringstream ss1(created);
    std::istringstream ss2(other.created);
    ss1 >> date::parse("%Y-%m-%dT%TZ", tp1);
    ss2 >> date::parse("%Y-%m-%dT%TZ", tp2);
    return tp1 < tp2;
}

json_t* NewPatchInfo::toJson()
{
    json_t* rootJ = json_object();

    json_object_set_new(rootJ, "id", json_integer(id));
    json_object_set_new(rootJ, "url", json_string(url.c_str()));
    json_object_set_new(rootJ, "slug", json_string(slug.c_str()));
    json_object_set_new(rootJ, "title", json_string(title.c_str()));
    json_object_set_new(rootJ, "excerpt", json_string(excerpt.c_str()));
    json_object_set_new(rootJ, "content", json_string(content.c_str()));
    json_object_set_new(rootJ, "revision", json_string(revision.c_str()));

    json_object_set_new(rootJ, "author", author.toJson());
    json_object_set_new(rootJ, "created", json_string(created.c_str()));

    json_t* filesJ = json_array();
    for (auto file : files) {
        json_array_append_new(filesJ, file.toJson());
    }
    json_object_set_new(rootJ, "files", filesJ);

    return rootJ;
}

void NewPatchInfo::fromJson(json_t* rootJ)
{
    if (rootJ == NULL) {
        return;
    }

    auto idJ = json_object_get(rootJ, "id");
    if (idJ) {
        id = json_integer_value(idJ);
    }

    auto urlJ = json_object_get(rootJ, "url");
    if (urlJ) {
        url = json_string_value(urlJ);
    }

    auto slugJ = json_object_get(rootJ, "slug");
    if (slugJ) {
        slug = json_string_value(slugJ);
    }

    auto titleJ = json_object_get(rootJ, "title");
    if (titleJ) {
        title = json_string_value(titleJ);
    }

    auto excerptJ = json_object_get(rootJ, "excerpt");
    if (excerptJ) {
        excerpt = json_string_value(excerptJ);
    }

    auto contentJ = json_object_get(rootJ, "content");
    if (contentJ) {
        content = json_string_value(contentJ);
    }

    auto revisionJ = json_object_get(rootJ, "revision");
    if (revisionJ) {
        revision = json_string_value(revisionJ);
    }

    auto authorJ = json_object_get(rootJ, "author");
    author.fromJson(authorJ);

    auto createdJ = json_object_get(rootJ, "created");
    if (createdJ) {
        created = json_string_value(createdJ);
    }

    auto filesJ = json_object_get(rootJ, "files");
    if (filesJ) {
        for (int i = 0; i < json_array_size(filesJ); i++) {
            json_t* fileJ = json_array_get(filesJ, i);
            PatchFile file;
            file.fromJson(fileJ);
            files.push_back(file);
        }
    }
}