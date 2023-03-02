#include "LibraryClient.hpp"

#include "clients/HttpClient.hpp"
#include "clients/storage/responses/TokenResponse.hpp"
#include "plugin.h"
#include "utils/logging.hpp"

using namespace rack;

LibraryClient::LibraryClient(std::string username, std::string password) : username(username), password(password) {
    token = settings::token;
}

LibraryClient::~LibraryClient() {}

Request LibraryClient::makeRequest(std::string endpoint) {
    auto request = Request();
    request.url = "https://api.vcvrack.com/" + endpoint;
    request.headers["User-Agent"] = "Rack/Prefabs";
    request.headers["Accept"] = "application/json";
    if (token.has_value()) {
        request.headers["Cookie"] = "token=" + *token;
    }

    request.cert = asset::plugin(pluginInstance, "res/cacert.pem");

    return request;
}

bool LibraryClient::login() {
    CINFO("LibraryClient::login() username: %s, password: %s", username.c_str(), password.c_str());

    auto request = makeRequest("/token");
    request.method = "POST";
    request.headers["Content-Type"] = "application/json";
    request.params["username"] = username;
    request.params["password"] = password;

    auto _resp = request.send();
    auto resp = TokenResponse(_resp.body);

    if (resp.code == "empty_response") {
        CINFO("LibraryClient::login() error: no response");
        return false;
    }

    if (resp.code != "") {
        CINFO("LibraryClient::login() error: %s : %s", resp.code.c_str(), resp.message.c_str());
        return false;
    }

    CINFO("LibraryClient::login() token: %s", resp.token.c_str());

    token.emplace(resp.token);
    return true;
}