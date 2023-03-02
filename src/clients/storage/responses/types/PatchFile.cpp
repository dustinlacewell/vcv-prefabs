#include "PatchFile.hpp"

json_t* PatchFile::toJson()
{
    json_t* rootJ = json_object();

    json_object_set_new(rootJ, "id", json_integer(id));
    json_object_set_new(rootJ, "url", json_string(url.c_str()));
    json_object_set_new(rootJ, "filesize", json_integer(filesize));
    json_object_set_new(rootJ, "filename", json_string(filename.c_str()));

    return rootJ;
}

void PatchFile::fromJson(json_t* rootJ)
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

    auto filesizeJ = json_object_get(rootJ, "filesize");
    if (filesizeJ) {
        filesize = json_integer_value(filesizeJ);
    }

    auto filenameJ = json_object_get(rootJ, "filename");
    if (filenameJ) {
        filename = json_string_value(filenameJ);
    }
}