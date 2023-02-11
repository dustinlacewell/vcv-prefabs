#include "efsw/efsw.hpp"

#include <plugin.hpp>
#include <rack.hpp>
#include <thread>

#include "UserStorageSource.hpp"
#include "client/StorageClient.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"

using namespace rack;

std::string url = "https://patchstorage.com/api/beta";

UserStorageSource::UserStorageSource(std::string username, std::string password, std::vector<StorageUser> users)
    : ArchiveSource("storage", asset::user("patch-storage")), username(username), password(password), users(users)
{
    SINFO("UserStorageSource::UserStorageSource() path: %s", path.c_str());
    SINFO("UserStorageSource::UserStorageSource() username: %s", username.c_str());
    SINFO("UserStorageSource::UserStorageSource() password: %s", password.c_str());
}

UserStorageSource::~UserStorageSource()
{
    SINFO("UserStorageSource::UserStorageSource() path: %s", path.c_str());
}

// this version of load waits while every patch is processed in individual threads
void UserStorageSource::refreshInThreads()
{
    auto client = StorageClient(username, password);
    auto loggedIn = client.login();

    if (!loggedIn) {
        SINFO("UserStorageSource::load() error: not logged in");
        return;
    }

    for (auto user : users) {
        auto userSlug = std::get<0>(user);
        auto userId = std::get<1>(user);
        SINFO("UserStorageSource::load() user: %s, %d", userSlug.c_str(), userId);

        auto patches = client.fetchUserPatches(userId);

        for (auto patch : patches) {
            SINFO("UserStorageSource::load() patch: %d, %s", patch.id, patch.slug.c_str());
            auto patchPath = asset::user("patch-storage") + "/" + userSlug + "/" + patch.slug;

            auto patchExists = rack::system::isDirectory(patchPath);

            if (patchExists) {
                SINFO("UserStorageSource::load() patch already exists: %s", patchPath.c_str());
                // send each file to this->readRack()
                auto files = allFiles(patchPath);
                auto patchFiles = filesWithExtension(files, "vcv");

                for (auto patchFile : patchFiles) {
                    SINFO("UserStorageSource::load() loading existing patch: %s", patchFile.c_str());
                    this->readRack(patchFile);
                }

                continue;
            }

            auto patchInfo = client.fetchPatchInfo(patch.id);
            if (!patchInfo.has_value()) {
                SINFO("UserStorageSource::load() error: no patchInfo for %s", patch.slug.c_str());
                continue;
            }

            auto files = (*patchInfo).files;

            for (auto file : files) {
                SINFO("UserStorageSource::load() file: %s, %s", file.url.c_str(), file.filename.c_str());

                auto extension = extensionFrom(file.filename);

                if (extension != "vcv") {
                    SINFO("UserStorageSource::load() skipping non-vcv file: %s", file.filename.c_str());
                    continue;
                }

                auto fullFilename =
                    asset::user("patch-storage") + "/" + userSlug + "/" + patch.slug + "/" + file.filename;

                auto alreadyExists = rack::system::isFile(fullFilename);

                if (alreadyExists) {
                    SINFO("UserStorageSource::load() already exists: %s", fullFilename.c_str());
                    this->readRack(fullFilename);
                    continue;
                }

                auto fileDownloaded = client.downloadPatch(file.url, fullFilename);
                if (fileDownloaded) {
                    SINFO("UserStorageSource::load() patchFile: %s", fullFilename.c_str());
                }
                else {
                    SINFO("UserStorageSource::load() error: no patchFile %s", fullFilename.c_str());
                }
            }
        }
    }
}

void UserStorageSource::load()
{
    ArchiveSource::load();
    // refreshInThreads();
}

Rack* UserStorageSource::read(std::string filename)
{
    auto rack = FileSource::read(filename);
    if (rack)
        rack->displayName = dirnameFrom(filename);
    return rack;
}