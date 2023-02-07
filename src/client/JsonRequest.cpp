#include "JsonRequest.hpp"
#include "consts.hpp"
#include "utils/logging.hpp"

using namespace rack::network;

std::string encodeParams(StringMap params)
{
    std::string result;
    if (params.size() == 0) {
        return "";
    }
    for (auto param : params) {
        result += param.first + "=" + encodeUrl(param.second);
        if (param != *params.rbegin()) {
            result += "&";
        }
    }
    return "?" + result;
}

JsonRequest::JsonRequest(std::string endpoint) : endpoint(endpoint)
{
    headers["User-Agent"] = "Rack/Prefabs";
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    headers["Accept"] = "application/json";
}

json_t* JsonRequest::send(Method method)
{
    auto dataJ = json_object();
    auto encodedParams = encodeParams(params);

    auto fullUrl = PATCH_STORAGE_URL + endpoint + encodedParams;

    CINFO("JsonRequest::send() fullUrl: %s", fullUrl.c_str());
    CINFO("JsonRequest::send() headers:");
    for (auto header : headers) {
        CINFO("JsonRequest::send() %s: %s", header.first.c_str(), header.second.c_str());
    }
    CINFO("JsonRequest::send() params:");
    for (auto param : params) {
        CINFO("JsonRequest::send() %s: %s", param.first.c_str(), param.second.c_str());
    }

    auto result = requestJson(method, fullUrl, dataJ, headers);
    if (!result) {
        CINFO("JsonRequest::send() received null from network::requestJson()");
    }

    return result;
}

json_t* JsonRequest::get()
{
    auto dataJ = json_object();
    // set params on dataJ
    for (auto param : params) {
        json_object_set_new(dataJ, param.first.c_str(), json_string(param.second.c_str()));
    }

    auto fullUrl = PATCH_STORAGE_URL + endpoint;

    CINFO("JsonRequest::send() fullUrl: %s", fullUrl.c_str());
    CINFO("JsonRequest::send() headers:");
    for (auto header : headers) {
        CINFO("JsonRequest::send() %s: %s", header.first.c_str(), header.second.c_str());
    }
    CINFO("JsonRequest::send() params:");
    for (auto param : params) {
        CINFO("JsonRequest::send() %s: %s", param.first.c_str(), param.second.c_str());
    }

    auto result = requestJson(Method::METHOD_GET, fullUrl, dataJ, headers);
    if (!result) {
        CINFO("JsonRequest::send() received null from network::requestJson()");
    }

    return result;
}

json_t* JsonRequest::post()
{
    return send(Method::METHOD_POST);
}