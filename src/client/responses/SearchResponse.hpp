#pragma once

#include <rack.hpp>
#include "utils/logging.hpp"

struct PatchResult {
    int id;
    std::string url;
    std::string slug;
    std::string title;

    PatchResult(json_t* itemJ)
    {
        // check for id
        auto idJ = json_object_get(itemJ, "id");
        if (idJ) {
            id = json_integer_value(idJ);
        }

        // check for url
        auto urlJ = json_object_get(itemJ, "url");
        if (urlJ) {
            url = json_string_value(urlJ);
        }

        // check for slug
        auto slugJ = json_object_get(itemJ, "slug");
        if (slugJ) {
            slug = json_string_value(slugJ);
        }

        // check for title
        auto titleJ = json_object_get(itemJ, "title");
        if (titleJ) {
            title = json_string_value(titleJ);
        }
    }
};

struct SearchResponse {
    std::vector<PatchResult> patches;

    std::string code;
    std::string message;

    SearchResponse(json_t* responseJ)
    {
        if (responseJ == NULL) {
            code = "empty_response";
            message = "Empty response.";
            return;
        }

        // check for error
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

        CINFO("itemsJ: %d", json_array_size(responseJ));
        for (int i = 0; i < json_array_size(responseJ); i++) {
            auto itemJ = json_array_get(responseJ, i);
            if (itemJ) {
                PatchResult patchResult(itemJ);
                patches.push_back(patchResult);
            }
        }
    }
};
