#pragma once

#include <rack.hpp>
#include "utils/logging.hpp"

#include "RackApiResponse.hpp"

struct TokenResponse : RackApiResponse {
    std::string token;

    TokenResponse(json_t* responseJ) : RackApiResponse(responseJ) {
        // check for token
        auto tokenJ = json_object_get(responseJ, "token");
        if (!tokenJ) {
            error = "No token in response.";
            return;
        }

        token = json_string_value(tokenJ);
    }
};