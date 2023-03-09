#pragma once

#include <optional>
#include <rack.hpp>
#include "utils/logging.hpp"

struct RackApiResponse {
    std::optional<std::string> error;

    RackApiResponse(json_t* responseJ) {
        if (!responseJ) {
            error = "Empty response.";
            return;
        }

        // check for error
        auto errorJ = json_object_get(responseJ, "error");
        if (errorJ) {
            error = json_string_value(errorJ);
            return;
        }
    }
};