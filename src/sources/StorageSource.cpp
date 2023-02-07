#include "efsw/efsw.hpp"

#include <plugin.hpp>
#include <rack.hpp>
#include <thread>

#include "StorageSource.hpp"
#include "client/StorageClient.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"

using namespace rack;

std::string url = "https://patchstorage.com/api/beta";

std::string authenticate(std::string username, std::string password)
{
    SINFO("StorageSource::authenticate() username: %s, password: %s", username.c_str(), password.c_str());

    network::Method method = network::Method::METHOD_POST;
    auto dataJ = json_object();

    auto encodedUsername = network::encodeUrl(username);
    auto encodedPassword = network::encodeUrl(password);

    auto urlParams = "username=" + encodedUsername + "&password=" + encodedPassword;
    auto fullUrl = url + "/auth/token" + "?" + urlParams;

    DINFO("StorageSource::authenticate() fullUrl: %s", fullUrl.c_str());

    auto userAgent = {"User-Agent", "Prefabs"};
    std::map<std::string, std::string> cookies = {{
                                                      "User-Agent",
                                                      "Prefabs",
                                                  },
        {
            "Content-Type",
            "application/x-www-form-urlencoded",

        },
        {
            "Accept",
            "application/json",
        }};

    // send makeRequest
    auto responseJ = network::requestJson(method, fullUrl, dataJ, cookies);

    // check for errors
    if (!responseJ) {
        SINFO("StorageSource::authenticate() error: no response");
        return "";
    }

    // check json for "code" indicating error
    auto codeJ = json_object_get(responseJ, "code");
    if (codeJ) {
        SINFO("StorageSource::authenticate() error: %s", json_string_value(codeJ));
        return "";
    }

    // check json for "token" indicating success
    auto tokenJ = json_object_get(responseJ, "token");
    if (!tokenJ) {
        SINFO("StorageSource::authenticate() error: no token");
        return "";
    }

    // return token
    return json_string_value(tokenJ);
}

std::vector<StoragePatch> getStoragePatches(std::string token, int userId, int page = 1)
{
    network::Method method = network::Method::METHOD_GET;

    auto authors = "authors=" + network::encodeUrl(std::to_string(userId));
    auto maxResults = "per_page=5";
    auto urlParams = authors + "&" + maxResults;
    auto fullUrl = url + "/patches" + "?" + urlParams;

    DINFO("StorageSource::getStoragePatches() fullUrl: %s", fullUrl.c_str());

    std::map<std::string, std::string> cookies;
    cookies["UserAgent"] = "Prefabs";
    cookies["Content-Type"] = "application/x-www-form-urlencoded";
    cookies["Accept"] = "application/json";
    cookies["Authorization"] = "Bearer" + token;

    // send makeRequest
    auto responseJ = network::requestJson(method, fullUrl, NULL, cookies);

    // check for errors
    if (!responseJ) {
        SINFO("StorageSource::getStoragePatches() error: no response");
        return {};
    }

    // resonposeJ is an array of { id, slug } objects
    auto patches = std::vector<StoragePatch>();
    for (int i = 0; i < json_array_size(responseJ); i++) {
        auto patchJ = json_array_get(responseJ, i);
        auto idJ = json_object_get(patchJ, "id");
        auto slugJ = json_object_get(patchJ, "slug");
        if (idJ && slugJ) {
            int id = json_integer_value(idJ);
            std::string slug = json_string_value(slugJ);
            patches.push_back(StoragePatch(slug, id));
        }
    }

    return patches;
}

// getPatchInfo requests /patches/{id} and returns a list of file ids
// in the response, the "files" key is an array of { url, filename } objects
std::vector<std::tuple<std::string, std::string>> getPatchInfo(std::string token, int patchId)
{
    network::Method method = network::Method::METHOD_GET;

    auto fullUrl = url + "/patches/" + std::to_string(patchId);

    DINFO("StorageSource::getPatchInfo() fullUrl: %s", fullUrl.c_str());

    std::map<std::string, std::string> cookies;
    cookies["UserAgent"] = "Prefabs";
    cookies["Accept"] = "application/json";
    cookies["Authorization"] = "Bearer" + token;

    // send makeRequest
    auto responseJ = network::requestJson(method, fullUrl, NULL, cookies);

    // check for errors
    if (!responseJ) {
        SINFO("StorageSource::getPatchInfo() error: no response");
        return {};
    }

    // check json for "code" indicating error
    auto codeJ = json_object_get(responseJ, "code");
    if (codeJ) {
        auto code = json_string_value(codeJ);
        // check code is not ""
        if (std::string(code) != "") {
            SINFO("StorageSource::getPatchInfo() error: %s", json_string_value(codeJ));
            char* responseStr = json_dumps(responseJ, JSON_INDENT(2));
            SINFO(responseStr);
            return {};
        }
    }

    // check json for "files" indicating success
    auto filesJ = json_object_get(responseJ, "files");
    if (!filesJ) {
        SINFO("StorageSource::getPatchInfo() error: no files");
        return {};
    }

    // filesJ is an array of { url, filename } objects
    auto files = std::vector<std::tuple<std::string, std::string>>();
    for (int i = 0; i < json_array_size(filesJ); i++) {
        auto fileJ = json_array_get(filesJ, i);
        auto urlJ = json_object_get(fileJ, "url");
        auto filenameJ = json_object_get(fileJ, "filename");
        if (urlJ && filenameJ) {
            std::string url = json_string_value(urlJ);
            std::string filename = json_string_value(filenameJ);
            files.push_back(std::make_tuple(url, filename));
        }
    }

    return files;
}

// getPatchFile requests the patch url and downloads it with requestFile
// the API for requestFile looks like
// bool requestDownload(const std::string& url, const std::string& filename, float* progress, const CookieMap& cookies = {});
bool getPatchFile(std::string token, std::string slug, std::string url, std::string filename)
{
    network::Method method = network::Method::METHOD_GET;

    DINFO("StorageSource::getPatchFile() url: %s", url.c_str());
    DINFO("StorageSource::getPatchFile() filename: %s", filename.c_str());

    std::map<std::string, std::string> cookies;
    cookies["UserAgent"] = "Prefabs";
    cookies["Accept"] = "application/json";
    cookies["Authorization"] = "Bearer" + token;

    float progress = 0.0f;

    auto fullPath = asset::user("patches") + "/" + slug + "/" + filename;

    // send makeRequest
    auto response = network::requestDownload(url, fullPath, &progress, cookies);

    // check for errors
    if (!response) {
        SINFO("StorageSource::getPatchFile() error: no response");
        return false;
    }

    return true;
}

StorageSource::StorageSource(std::string username, std::string password, std::vector<StorageUser> users)
    : ArchiveSource("storage", asset::user("patch-storage")), username(username), password(password), users(users)
{
    SINFO("StorageSource::StorageSource() path: %s", path.c_str());
    SINFO("StorageSource::StorageSource() username: %s", username.c_str());
    SINFO("StorageSource::StorageSource() password: %s", password.c_str());
}

StorageSource::~StorageSource()
{
    SINFO("StorageSource::StorageSource() path: %s", path.c_str());
}

//void StorageSource::refresh()
//{
//    auto client = StorageClient(username, password);
//    auto loggedIn = client.login();
//
//    if (!loggedIn) {
//        SINFO("StorageSource::refresh() error: not logged in");
//        return;
//    }
//
//    for (auto user : users) {
//        auto userSlug = std::get<0>(user);
//        auto userId = std::get<1>(user);
//        SINFO("StorageSource::refresh() user: %s, %d", userSlug.c_str(), userId);
//
//        auto patches = client.fetchUserPatches(userId);
//
//        for (auto patch : patches) {
//            SINFO("StorageSource::refresh() patch: %d, %s", patch.id, patch.slug.c_str());
//
//            auto patchInfo = client.fetchPatchInfo(patch.id);
//            auto files = patchInfo.value().files;
//            for (auto file : files) {
//                SINFO("StorageSource::refresh() file: %s, %s", file.url.c_str(), file.filename.c_str());
//                auto fullFilename = asset::user("patch-storage") + "/" + userSlug + "/" + file.filename;
//                auto alreadyExists = rack::system::isFile(fullFilename);
//
//                if (alreadyExists) {
//                    SINFO("StorageSource::refresh() already exists: %s", fullFilename.c_str());
//                    this->readRack(userSlug, file.filename);
//                    continue;
//                }
//
//                auto fileDownloaded = client.downloadPatch(file.url, fullFilename);
//                if (fileDownloaded) {
//                    SINFO("StorageSource::refresh() patchFile: %s", fullFilename.c_str());
//                    // filename without extension
//                    auto filename = file.filename.substr(0, file.filename.find_last_of("."));
//                    this->readRack(userSlug, filename);
//                }
//                else {
//                    SINFO("StorageSource::refresh() error: no patchFile %s", fullFilename.c_str());
//                }
//            }
//        }
//    }
//}

// this version of refresh waits while every patch is processed in individual threads
void StorageSource::refreshInThreads()
{
    auto client = StorageClient(username, password);
    auto loggedIn = client.login();

    if (!loggedIn) {
        SINFO("StorageSource::refresh() error: not logged in");
        return;
    }

    for (auto user : users) {
        auto userSlug = std::get<0>(user);
        auto userId = std::get<1>(user);
        SINFO("StorageSource::refresh() user: %s, %d", userSlug.c_str(), userId);

        auto patches = client.fetchUserPatches(userId);

        for (auto patch : patches) {
            SINFO("StorageSource::refresh() patch: %d, %s", patch.id, patch.slug.c_str());
            auto patchPath = asset::user("patch-storage") + "/" + userSlug + "/" + patch.slug;

            auto patchExists = rack::system::isDirectory(patchPath);

            if (patchExists) {
                SINFO("StorageSource::refresh() patch already exists: %s", patchPath.c_str());
                // send each file to this->readRack()
                auto files = allFiles(patchPath);
                auto patchFiles = filesWithExtension(files, "vcv");

                for (auto patchFile : patchFiles) {
                    SINFO("StorageSource::refresh() loading existing patch: %s", patchFile.c_str());
                    this->readRack(patchFile);
                }

                continue;
            }

            auto patchInfo = client.fetchPatchInfo(patch.id);
            auto files = patchInfo.value().files;

            // create a thread for this patch
            // run each of the file downloads of this patch in that thread

            // create thread now, run file processing in lambda
            std::thread t([=]() {
                for (auto file : files) {
                    SINFO("StorageSource::refresh() file: %s, %s", file.url.c_str(), file.filename.c_str());

                    auto fullFilename =
                        asset::user("patch-storage") + "/" + userSlug + "/" + patch.slug + "/" + file.filename;

                    auto alreadyExists = rack::system::isFile(fullFilename);

                    if (alreadyExists) {
                        SINFO("StorageSource::refresh() already exists: %s", fullFilename.c_str());
                        this->readRack(fullFilename);
                        continue;
                    }

                    auto fileDownloaded = client.downloadPatch(file.url, fullFilename);
                    if (fileDownloaded) {
                        SINFO("StorageSource::refresh() patchFile: %s", fullFilename.c_str());
                        this->readRack(fullFilename);
                    }
                    else {
                        SINFO("StorageSource::refresh() error: no patchFile %s", fullFilename.c_str());
                    }
                }
            });

            // detach thread so it runs in the background
            t.detach();
        }
    }
}

void StorageSource::refresh()
{
    refreshInThreads();
}