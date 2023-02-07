#pragma once

#include <rack.hpp>

#include "typedefs.hpp"

struct JsonRequest {
    StringMap headers;
    StringMap params;

    std::string endpoint;

    JsonRequest(std::string endpoint);

    json_t* send(rack::network::Method method);
    json_t* get();
    json_t* post();
};