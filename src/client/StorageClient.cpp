#include "StorageClient.hpp"

#include "HttpClient.hpp"
#include "plugin.h"
#include "responses/PatchInfoResponse.hpp"
#include "responses/TokenResponse.hpp"
#include "utils/logging.hpp"

using namespace rack;

StorageClient::StorageClient(std::string username, std::string password) : username(username), password(password) {}

StorageClient::~StorageClient() {}

Request StorageClient::makeRequest(std::string endpoint)
{
    auto request = Request();
    request.url = "https://patchstorage.com/api/beta" + endpoint;
    request.headers["User-Agent"] = "Rack/Prefabs";
    request.headers["Accept"] = "application/json";
    if (token.has_value()) {
        request.headers["Authorization"] = "Bearer " + token.value();
    }

    request.cert = asset::plugin(pluginInstance, "res/cacert.pem");

    return request;
}

bool StorageClient::login()
{
    CINFO("StorageClient::login() username: %s, password: %s", username.c_str(), password.c_str());

    auto request = makeRequest("/auth/token");
    request.method = "POST";
    request.headers["Content-Type"] = "application/json";
    request.params["username"] = username;
    request.params["password"] = password;

    auto _resp = request.send();
    auto resp = TokenResponse(_resp.body);

    if (resp.code == "empty_response") {
        CINFO("StorageClient::login() error: no response");
        return false;
    }

    if (resp.code != "") {
        CINFO("StorageClient::login() error: %s : %s", resp.code.c_str(), resp.message.c_str());
        return false;
    }

    CINFO("StorageClient::login() token: %s", resp.token.c_str());

    token.emplace(resp.token);
    return true;
}

std::vector<PatchResult> StorageClient::fetchUserPatches(int authorId)
{
    CINFO("StorageClient::fetchUserPatches() authorId: %d", authorId);

    auto request = makeRequest("/patches");
    request.method = "GET";
    request.params["authors"] = network::encodeUrl(std::to_string(authorId));
    request.params["platforms"] = "745";
    request.params["per_page"] = "100";

    auto _resp = request.send();

    // dump _resp.body to string from json
    std::string str = json_dumps(_resp.body, 0);
    CINFO("StorageClient::fetchUserPatches() _resp.body:");
    CINFO(str.c_str());

    auto resp = SearchResponse(_resp.body);

    if (resp.code == "empty_response") {
        CINFO("StorageClient::fetchUserPatches() error: no response");
        return {};
    }

    if (resp.code != "") {
        CINFO("StorageClient::fetchUserPatches() error: %s", resp.code.c_str());
        return {};
    }

    return resp.patches;
}

std::optional<PatchInfo> StorageClient::fetchPatchInfo(int patchId)
{
    CINFO("StorageClient::fetchPatchInfo() patchId: %d", patchId);

    auto request = makeRequest("/patches/" + std::to_string(patchId));
    auto _resp = request.send();
    auto resp = PatchInfoResponse(_resp.body);

    if (resp.code == "empty_response") {
        CINFO("StorageClient::fetchPatchInfo() error: no response");
        return {};
    }

    if (resp.code != "") {
        CINFO("StorageClient::fetchPatchInfo() error: %s", resp.code.c_str());
        return {};
    }

    return resp.patchInfo;
}

bool StorageClient::downloadPatch(std::string url, std::string path) const
{
    if (!token.has_value()) {
        throw std::runtime_error("StorageClient::downloadPatch() token is not set");
    }

    auto request = DownloadRequest();
    request.url = url;
    request.headers["User-Agent"] = "Rack/Prefabs";
    request.headers["Accept"] = "application/json";
    request.headers["Authorization"] = "Bearer " + token.value();
    request.cert = asset::plugin(pluginInstance, "res/cacert.pem");
    request.filename = path;

    // find parent directory of path using substr
    auto parentDir = path.substr(0, path.find_last_of("/"));

    // ensure every directory in the path exists
    // system::createDirectory does not create intermediate directories
    // we must start at the root and create each directory in the path

    std::string dir = "";
    for (auto& part : string::split(parentDir, "/")) {
        dir += part + "/";
        system::createDirectory(dir);
    }

    auto resp = request.send();

    if (resp.status != 200) {
        CINFO("StorageClient::downloadPatch() error: %d", resp.status);
        return false;
    }

    // if the file was a .zip, decompress it
    if (string::endsWith(request.filename, ".zip")) {
        auto zipPath = request.filename;
        auto dirPath = request.filename.substr(0, path.find_last_of("/"));
        system::createDirectory(dirPath);
        system::unarchiveToDirectory(zipPath, dirPath);
        // delete file with non-system:: functions
        std::remove(zipPath.c_str());
    }

    return true;
}
