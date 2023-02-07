#pragma once

#include <rack.hpp>
#include "utils/logging.hpp"

struct TokenResponse {
    std::string token;
    std::string email;
    std::string username;
    std::string display_name;

    std::string code;
    std::string message;

    TokenResponse(json_t* responseJ)
    {

        if (!responseJ) {
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

        // check for token
        auto tokenJ = json_object_get(responseJ, "token");
        if (tokenJ) {
            token = json_string_value(tokenJ);
        }

        // check for email
        auto emailJ = json_object_get(responseJ, "email");
        if (emailJ) {
            email = json_string_value(emailJ);
        }

        // check for username
        auto usernameJ = json_object_get(responseJ, "username");
        if (usernameJ) {
            username = json_string_value(usernameJ);
        }

        // check for display_name
        auto display_nameJ = json_object_get(responseJ, "display_name");
        if (display_nameJ) {
            display_name = json_string_value(display_nameJ);
        }
    }
};