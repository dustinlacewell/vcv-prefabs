#pragma once

#include <optional>

#include <rack.hpp>
#include "clients/storage/responses/types/NewPatchInfo.hpp"

struct PatchInfoResponse {
    std::optional<NewPatchInfo> patchInfo;
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

        patchInfo = NewPatchInfo();
        patchInfo->fromJson(responseJ);
    }
};