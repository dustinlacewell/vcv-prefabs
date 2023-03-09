#pragma once

#include <rack.hpp>
#include "utils/logging.hpp"

#include "RackApiResponse.hpp"

struct AddModuleResponse : RackApiResponse {
    AddModuleResponse(json_t* responseJ) : RackApiResponse(responseJ) {
        //        // check for token
        //        auto tokenJ = json_object_get(responseJ, "success");
        //        if (!tokenJ) {
        //            error = "No success.";
        //            return;
        //        }
    }
};