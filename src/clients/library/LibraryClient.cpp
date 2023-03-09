#include "LibraryClient.hpp"
#include <condition_variable>
#include <library.hpp>
#include <plugin.hpp>

#include "clients/HttpClient.hpp"
#include "clients/library/responses/AddModuleResponse.hpp"
#include "clients/storage/responses/TokenResponse.hpp"
#include "plugin.h"
#include "utils/logging.hpp"

using namespace rack;

static std::mutex appUpdateMutex;
static std::mutex updateMutex;
static std::mutex timeoutMutex;
static std::condition_variable updateCv;

//LibraryClient::LibraryClient(std::string username, std::string password) : username(username), password(password) {
//    token = settings::token;
//}
//
//LibraryClient::~LibraryClient() {}

Request LibraryClient::makeRequest(std::string endpoint) {
    auto request = Request();
    request.url = "https://api.vcvrack.com/" + endpoint;
    request.headers["User-Agent"] = "Rack/Prefabs";
    request.headers["Accept"] = "*/*";
    request.headers["Cookie"] = "token=" + settings::token;
    request.cert = asset::plugin(pluginInstance, "res/cacert.pem");

    return request;
}

//bool LibraryClient::login() {
//    CINFO("LibraryClient::login() username: %s, password: %s", username.c_str(), password.c_str());
//
//    auto request = makeRequest("/token");
//    request.method = "POST";
//    request.headers["Content-Type"] = "application/json";
//    request.params["username"] = username;
//    request.params["password"] = password;
//
//    auto _resp = request.send();
//    auto resp = TokenResponse(_resp.body);
//
//    if (resp.code == "empty_response") {
//        CINFO("LibraryClient::login() error: no response");
//        return false;
//    }
//
//    if (resp.code != "") {
//        CINFO("LibraryClient::login() error: %s : %s", resp.code.c_str(), resp.message.c_str());
//        return false;
//    }
//
//    CINFO("LibraryClient::login() token: %s", resp.token.c_str());
//
//    token.emplace(resp.token);
//    return true;
//}

bool LibraryClient::addModule(std::string pluginSlug, std::string moduleSlug) {
    CINFO("LibraryClient::addModule() pluginSlug: %s, moduleSlug: %s", pluginSlug.c_str(), moduleSlug.c_str());

    auto request = makeRequest("modules");
    request.method = "POST";
    request.encoded = false;
    request.headers["Content-Type"] = "application/json";
    request.params["pluginSlug"] = pluginSlug;
    request.params["moduleSlug"] = moduleSlug;

    auto _resp = request.send();
    auto resp = AddModuleResponse(_resp.body);

    if (resp.error.has_value()) {
        DINFO("LibraryClient::addModule() error: %s", resp.error->c_str());
        return false;
    }

    return true;
}

bool LibraryClient::removeModule(std::string pluginSlug, std::string moduleSlug) {
    CINFO("LibraryClient::addModule() pluginSlug: %s, moduleSlug: %s", pluginSlug.c_str(), moduleSlug.c_str());

    auto request = makeRequest("modules");
    request.method = "DELETE";
    request.encoded = false;
    request.headers["Content-Type"] = "application/json";
    request.params["pluginSlug"] = pluginSlug;
    request.params["moduleSlug"] = moduleSlug;

    auto _resp = request.send();
    auto resp = AddModuleResponse(_resp.body);

    if (resp.error.has_value()) {
        CINFO("LibraryClient::addModule() error: %s", resp.error->c_str());
        return false;
    }

    return true;
}

bool LibraryClient::subscribe(std::string pluginSlug) {
    CINFO("LibraryClient::subscribe() pluginSlug: %s", pluginSlug.c_str());

    auto request = makeRequest("plugins/");
    request.method = "POST";
    request.encoded = false;
    request.headers["Content-Type"] = "application/json";
    request.params["pluginSlug"] = pluginSlug;

    auto _resp = request.send();
    auto resp = AddModuleResponse(_resp.body);

    if (resp.error.has_value()) {
        CINFO("LibraryClient::subscribe() error: %s", resp.error->c_str());
        return false;
    }

    return true;
}

bool LibraryClient::unsubscribe(std::string pluginSlug) {
    CINFO("LibraryClient::unsubscribe() pluginSlug: %s", pluginSlug.c_str());

    auto request = makeRequest("plugins/");
    request.method = "DELETE";
    request.encoded = false;
    request.headers["Content-Type"] = "application/json";
    request.params["pluginSlug"] = pluginSlug;

    auto _resp = request.send();
    auto resp = AddModuleResponse(_resp.body);

    if (resp.error.has_value()) {
        CINFO("LibraryClient::unsubscribe() error: %s", resp.error->c_str());
        return false;
    }

    return true;
}

//void LibraryClient::syncUpdates() {
//    if (settings::token.empty())
//        return;
//
//    // updateInfos could possibly change in the checkUpdates() thread, but checkUpdates() will not execute if syncUpdate() is running, so the chance of the updateInfos map being modified while iterating is rare.
//    auto updateInfosClone = library::updateInfos;
//    for (auto& pair : updateInfosClone) {
//        syncUpdate(pair.first);
//    }
//    library::restartRequested = true;
//}

void LibraryClient::checkUpdates() {
    if (!updateMutex.try_lock())
        return;
    DEFER({ updateMutex.unlock(); });

    if (settings::token.empty())
        return;

    // Refuse to check for updates while updating plugins
    if (library::isSyncing)
        return;

    library::updateStatus = "Querying for updates...";

    // port the commented above code to the local LibraryClient style of requests
    auto request = makeRequest("user");
    request.method = "GET";
    auto _resp = request.send();
    auto resp = RackApiResponse(_resp.body);
    if (resp.error.has_value()) {
        WARN("Request for user account failed");
        library::updateStatus = "Could not query user account";
        return;
    }

    // port the commented above code to the local LibraryClient style of requests
    auto manifestRequest = makeRequest("library/manifests");
    manifestRequest.method = "GET";
    manifestRequest.encoded = false;
    manifestRequest.headers["Content-Type"] = "application/json";
    manifestRequest.params["version"] = APP_VERSION_MAJOR;
    auto _manifestResp = manifestRequest.send();
    auto manifestResp = RackApiResponse(_manifestResp.body);
    if (manifestResp.error.has_value()) {
        WARN("Request for library manifests failed");
        library::updateStatus = "Could not query plugin manifests";
        return;
    }

    // port the commented above code to the local LibraryClient style of requests
    auto modulesRequest = makeRequest("modules");
    modulesRequest.method = "GET";
    auto _modulesResp = modulesRequest.send();
    auto modulesResp = RackApiResponse(_modulesResp.body);
    if (modulesResp.error.has_value()) {
        WARN("Request for user's modules failed");
        library::updateStatus = "Could not query user's modules";
        return;
    }

    json_t* manifestsJ = json_object_get(_manifestResp.body, "manifests");
    json_t* pluginsJ = json_object_get(_modulesResp.body, "modules");

    const char* modulesKey;
    json_t* modulesJ;
    json_object_foreach(pluginsJ, modulesKey, modulesJ) {
        std::string pluginSlug = modulesKey;
        // Get plugin manifest
        json_t* manifestJ = json_object_get(manifestsJ, pluginSlug.c_str());
        if (!manifestJ) {
            // Skip plugin silently
            continue;
        }

        // Don't replace existing UpdateInfo, even if version is newer.
        // This keeps things sane and ensures that only one version of each plugin is downloaded to `plugins/` at a time.
        auto it = library::updateInfos.find(pluginSlug);
        if (it != library::updateInfos.end()) {
            continue;
        }
        library::UpdateInfo update;

        // Get plugin name
        json_t* nameJ = json_object_get(manifestJ, "name");
        if (nameJ)
            update.name = json_string_value(nameJ);

        // Get version
        json_t* versionJ = json_object_get(manifestJ, "version");
        if (!versionJ) {
            // WARN("Plugin %s has no version in manifest", pluginSlug.c_str());
            continue;
        }
        update.version = json_string_value(versionJ);
        // Reject plugins with ABI mismatch
        if (!string::startsWith(update.version, APP_VERSION_MAJOR + ".")) {
            continue;
        }

        // Check that update is needed
        plugin::Plugin* p = plugin::getPlugin(pluginSlug);
        if (p) {
            if (update.version == p->version)
                continue;
            if (string::Version(update.version) < string::Version(p->version))
                continue;
        }

        // Check that plugin is available for this arch
        json_t* archesJ = json_object_get(manifestJ, "arches");
        if (!archesJ)
            continue;
        std::string arch = APP_OS + "-" + APP_CPU;
        json_t* archJ = json_object_get(archesJ, arch.c_str());
        if (!json_boolean_value(archJ))
            continue;

        // Get changelog URL
        json_t* changelogUrlJ = json_object_get(manifestJ, "changelogUrl");
        if (changelogUrlJ)
            update.changelogUrl = json_string_value(changelogUrlJ);

        // Add update to updates map
        library::updateInfos[pluginSlug] = update;
    }

    // Merge module whitelist
    {
        // Clone plugin slugs from settings to temporary whitelist.
        // This makes existing plugins entirely hidden if removed from user's VCV account.
        std::map<std::string, settings::PluginWhitelist> moduleWhitelist;
        for (const auto& pluginPair : settings::moduleWhitelist) {
            std::string pluginSlug = pluginPair.first;
            moduleWhitelist[pluginSlug] = settings::PluginWhitelist();
        }

        // Iterate plugins
        const char* modulesKey;
        json_t* modulesJ;
        json_object_foreach(pluginsJ, modulesKey, modulesJ) {
            std::string pluginSlug = modulesKey;
            settings::PluginWhitelist& pw = moduleWhitelist[pluginSlug];

            // If value is "true", plugin is subscribed
            if (json_is_true(modulesJ)) {
                pw.subscribed = true;
                continue;
            }

            // Iterate modules in plugin
            size_t moduleIndex;
            json_t* moduleSlugJ;
            json_array_foreach(modulesJ, moduleIndex, moduleSlugJ) {
                std::string moduleSlug = json_string_value(moduleSlugJ);
                // Insert module in whitelist
                pw.moduleSlugs.insert(moduleSlug);
            }
        }

        settings::moduleWhitelist = moduleWhitelist;
    }

    library::updateStatus = "";
    library::refreshRequested = true;
}