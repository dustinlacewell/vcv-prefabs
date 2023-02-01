#include <asset.hpp>
#include <system.hpp>

#include "PatchSource.hpp"
#include "PrefabSource.hpp"
#include "utils/files.hpp"

using namespace rack;

PatchSource::PatchSource() : PrefabSource() {}
PatchSource::PatchSource(Plugin* plugin) : PrefabSource(plugin) {}

json_t* PatchSource::loadFile(std::string path)
{
    std::string extension = extensionFrom(path);

    if (extension != "vcv") {
        return nullptr;
    }

    if (settings::devMode) {
        INFO("[Prefabs] Loading patch from %s", path.c_str());
    }

    auto patchRoot = asset::user("prefab-tmp");
    system::createDirectories(patchRoot);

    if (settings::devMode) {
        INFO("[Prefabs] Unpacking patch to %s", patchRoot.c_str());
    }
    system::unarchiveToDirectory(path, patchRoot);

    auto patchPath = system::join(patchRoot, "patch.json");
    FILE* file = std::fopen(patchPath.c_str(), "r");
    if (!file)
        throw Exception("Could not open prefab patch %s", patchPath.c_str());
    DEFER({ std::fclose(file); });

    json_error_t error;
    json_t* rootJ = json_loadf(file, 0, &error);
    if (!rootJ)
        throw Exception("Failed to load patch. JSON parsing error at %s %d:%d %s",
            error.source,
            error.line,
            error.column,
            error.text);

    if (!rootJ) {
        return nullptr;
    }

    return rootJ;
}

std::string PatchSource::rootPath()
{
    auto path = asset::user("patches");
    if (plugin) {
        path = asset::plugin(plugin, "res/patches");
    }
    return path;
}