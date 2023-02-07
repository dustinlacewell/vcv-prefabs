#pragma once

#include <optional>

#include <rack.hpp>

struct PatchFileInfo {
    int id;
    std::string url;
    int filesize;
    std::string filename;

    PatchFileInfo(json_t* fileJ)
    {
        if (fileJ == NULL) {
            return;
        }

        auto idJ = json_object_get(fileJ, "id");
        if (idJ) {
            id = json_integer_value(idJ);
        }

        auto urlJ = json_object_get(fileJ, "url");
        if (urlJ) {
            url = json_string_value(urlJ);
        }

        auto filesizeJ = json_object_get(fileJ, "filesize");
        if (filesizeJ) {
            filesize = json_integer_value(filesizeJ);
        }

        auto filenameJ = json_object_get(fileJ, "filename");
        if (filenameJ) {
            filename = json_string_value(filenameJ);
        }
    }
};

struct PatchInfo {
    int id;
    std::string url;
    std::string slug;
    std::string title;
    std::string excerpt;
    std::string content;
    std::string code;
    std::vector<PatchFileInfo> files;
    std::string revision;

    PatchInfo(json_t* responseJ)
    {
        if (responseJ == NULL) {
            code = "empty_response";
            return;
        }

        // check for error
        auto codeJ = json_object_get(responseJ, "code");
        if (codeJ) {
            code = json_string_value(codeJ);

            if (code != "") {
                return;
            }
        }

        // check for id
        auto idJ = json_object_get(responseJ, "id");
        if (idJ) {
            id = json_integer_value(idJ);
        }

        // check for url
        auto urlJ = json_object_get(responseJ, "url");
        if (urlJ) {
            url = json_string_value(urlJ);
        }

        // check for slug
        auto slugJ = json_object_get(responseJ, "slug");
        if (slugJ) {
            slug = json_string_value(slugJ);
        }

        // check for title
        auto titleJ = json_object_get(responseJ, "title");
        if (titleJ) {
            title = json_string_value(titleJ);
        }

        // check for excerpt
        auto excerptJ = json_object_get(responseJ, "excerpt");
        if (excerptJ) {
            excerpt = json_string_value(excerptJ);
        }

        // check for content
        auto contentJ = json_object_get(responseJ, "content");
        if (contentJ) {
            content = json_string_value(contentJ);
        }

        // check for files
        auto filesJ = json_object_get(responseJ, "files");
        if (filesJ) {
            int size = json_array_size(filesJ);
            for (int i = 0; i < size; i++) {
                auto fileJ = json_array_get(filesJ, i);
                PatchFileInfo file(fileJ);
                files.push_back(file);
            }
        }

        // check for revision
        auto revisionJ = json_object_get(responseJ, "revision");
        if (revisionJ) {
            revision = json_string_value(revisionJ);
        }
    }
};

struct PatchInfoResponse {
    std::optional<PatchInfo> patchInfo;
    std::string code;
    std::string message;

    PatchInfoResponse(json_t* responseJ)
    {
        if (responseJ == NULL) {
            code = "empty_response";
            message = "Empty response.";
        }

        auto codeJ = json_object_get(responseJ, "code");
        if (codeJ) {
            code = json_string_value(codeJ);

            if (code != "") {
                auto messageJ = json_object_get(responseJ, "message");
                if (messageJ) {
                    message = json_string_value(messageJ);
                }
                return;
            }
        }

        patchInfo = PatchInfo(responseJ);
    }
};