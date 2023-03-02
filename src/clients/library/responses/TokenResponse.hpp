#pragma once

#include <rack.hpp>
#include "utils/logging.hpp"

struct TokenResponse {
    std::string token;
    std::string message;

    TokenResponse(json_t* responseJ) {
        if (!responseJ) {
            message = "Empty response.";
            return;
        }

        // check for error
        auto errorJ = json_object_get(responseJ, "error");
        if (errorJ) {
            message = json_string_value(errorJ);
            return;
        }

        // check for token
        auto tokenJ = json_object_get(responseJ, "token");
        if (!tokenJ) {
            message = "No token in response.";
            return;
        }

        token = json_string_value(tokenJ);
    }
};