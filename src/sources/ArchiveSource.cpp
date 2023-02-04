#include <asset.hpp>
#include <system.hpp>

#include "ArchiveSource.hpp"
#include "utils/files.hpp"
#include "utils/logging.hpp"
#include "utils/patches.hpp"

using namespace rack;

ArchiveSource::ArchiveSource(std::string slug, std::string root) : FileSource(slug, root) {}

json_t* ArchiveSource::readJson(std::string path)
{
    std::string extension = extensionFrom(path);

    if (extension != "vcv") {
        return nullptr;
    }

    DINFO("[Prefabs] Loading patch from %s", path.c_str());

    auto patchRoot = asset::user("prefabs-tmp");
    system::createDirectories(patchRoot);
    auto patchPath = system::join(patchRoot, "patch.json");

    if (isPatchLegacyV1(path)) {
        DINFO("[Prefabs] Patch is legacy: %s", path.c_str());
        system::copy(path, system::join(path, patchPath));
    }
    else {
        DINFO("[Prefabs] Unpacking patch to %s", patchRoot.c_str());
        try {
            system::unarchiveToDirectory(path, patchRoot);
        }
        catch (std::exception& e) {
            DINFO("[Prefabs] Failed to unpack patch: %s", e.what());
            return nullptr;
        }
    }

    FILE* file = std::fopen(patchPath.c_str(), "r");
    if (!file) {
        DINFO("[Prefabs] Could not open patch %s", patchPath.c_str());
        return nullptr;
    }
    DEFER({ std::fclose(file); });

    json_error_t error;
    json_t* rootJ = json_loadf(file, 0, &error);
    if (!rootJ) {
        if (settings::devMode) {
            DINFO("[Prefabs] Failed to load patch. JSON parsing error at %s %d:%d %s",
                error.source,
                error.line,
                error.column,
                error.text);
        }
        return nullptr;
    }

    return rootJ;
}