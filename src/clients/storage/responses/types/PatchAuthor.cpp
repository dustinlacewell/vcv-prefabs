#include "PatchAuthor.hpp"

json_t* PatchAuthor::toJson()
{
    json_t* rootJ = json_object();

    json_object_set_new(rootJ, "id", json_integer(id));
    json_object_set_new(rootJ, "slug", json_string(slug.c_str()));
    json_object_set_new(rootJ, "name", json_string(name.c_str()));

    return rootJ;
}

void PatchAuthor::fromJson(json_t* rootJ)
{
    if (rootJ == NULL) {
        return;
    }

    auto idJ = json_object_get(rootJ, "id");
    if (idJ) {
        id = json_integer_value(idJ);
    }

    auto slugJ = json_object_get(rootJ, "slug");
    if (slugJ) {
        slug = json_string_value(slugJ);
    }

    auto nameJ = json_object_get(rootJ, "name");
    if (nameJ) {
        name = json_string_value(nameJ);
    }
}