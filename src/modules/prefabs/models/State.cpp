#include <rack.hpp>

#include "State.hpp"
#include "utils/logging.hpp"

using namespace rack::dsp;

State::State() {
    browserMode = false;
    monoIcon = false;
    layout = TABBED;

    searchResultsQuantity = SimpleQuantity();
    searchResultsQuantity.label = "Number of search results";
    searchResultsQuantity.minValue = 8;
    searchResultsQuantity.maxValue = 128;
    searchResultsQuantity.defaultValue = 32;
    searchResultsQuantity.value = 32;
    searchResultsQuantity.rounded = true;

    colorQuantity = SimpleQuantity();
    colorQuantity.label = "Icon color";
    colorQuantity.minValue = 0;
    colorQuantity.maxValue = 1;
    colorQuantity.defaultValue = .25;
    colorQuantity.value = .25;
    colorQuantity.rounded = false;

    discoSpeedQuantity = SimpleQuantity();
    discoSpeedQuantity.label = "Disco maxSpeed";
    discoSpeedQuantity.minValue = 0.0f;
    discoSpeedQuantity.maxValue = 0.03f;
    discoSpeedQuantity.defaultValue = 0.0;
    discoSpeedQuantity.value = 0.0;
    discoSpeedQuantity.rounded = false;

    tagManager = ModuleTagManager();
}

json_t* State::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "browserMode", json_boolean(browserMode));
    json_object_set_new(rootJ, "layout", json_integer(layout));
    json_object_set_new(rootJ, "showing", json_boolean(showing));
    json_object_set_new(rootJ, "pos", json_pack("[f, f]", pos.x, pos.y));
    json_object_set_new(rootJ, "searchResultsQuantity", searchResultsQuantity.toJson());
    json_object_set_new(rootJ, "monoIcon", json_boolean(monoIcon));
    json_object_set_new(rootJ, "colorQuantity", colorQuantity.toJson());
    json_object_set_new(rootJ, "discoSpeedQuantity", discoSpeedQuantity.toJson());
    json_object_set_new(rootJ, "tagManager", tagManager.toJson());

    json_t* extraSourcesJ = json_array();
    for (auto& extraSource : extraPrefabSources) {
        json_t* extraSourceJ = json_pack("[s, s]", std::get<0>(extraSource).c_str(), std::get<1>(extraSource).c_str());
        json_array_append_new(extraSourcesJ, extraSourceJ);
    }
    json_object_set_new(rootJ, "extraPrefabSources", extraSourcesJ);

    json_t* extraPatchSourcesJ = json_array();
    for (auto& extraSource : extraPatchSources) {
        json_t* extraSourceJ = json_pack("[s, s]", std::get<0>(extraSource).c_str(), std::get<1>(extraSource).c_str());
        json_array_append_new(extraPatchSourcesJ, extraSourceJ);
    }
    json_object_set_new(rootJ, "extraPatchSources", extraPatchSourcesJ);

    json_t* storageUsersJ = json_array();
    for (auto& storageUser : storageUsers) {
        // each item is a str, int tuple pair
        json_t* storageUserJ = json_pack("[s, i]", std::get<0>(storageUser).c_str(), std::get<1>(storageUser));
        json_array_append_new(storageUsersJ, storageUserJ);
    }
    json_object_set_new(rootJ, "storageUsers", storageUsersJ);

    json_object_set_new(rootJ, "storageUsername", json_string(storageUsername.c_str()));
    json_object_set_new(rootJ, "storagePassword", json_string(storagePassword.c_str()));

    return rootJ;
}

void State::fromJson(json_t* rootJ) {
    json_t* browserJ = json_object_get(rootJ, "browserMode");
    if (browserJ)
        browserMode = json_boolean_value(browserJ);

    json_t* layoutJ = json_object_get(rootJ, "layout");
    if (layoutJ)
        layout = (LayoutType)json_integer_value(layoutJ);

    json_t* showJ = json_object_get(rootJ, "showing");
    if (showJ)
        showing = json_boolean_value(showJ);

    json_t* posJ = json_object_get(rootJ, "pos");
    if (posJ)
        pos = rack::Vec(json_real_value(json_array_get(posJ, 0)), json_real_value(json_array_get(posJ, 1)));

    json_t* searchResultsQuantityJ = json_object_get(rootJ, "searchResultsQuantity");
    if (searchResultsQuantityJ)
        searchResultsQuantity.fromJson(searchResultsQuantityJ);

    json_t* monoJ = json_object_get(rootJ, "monoIcon");
    if (monoJ)
        monoIcon = json_boolean_value(monoJ);

    json_t* colorQuantityJ = json_object_get(rootJ, "colorQuantity");
    if (colorQuantityJ)
        colorQuantity.fromJson(colorQuantityJ);

    json_t* discoSpeedQuantityJ = json_object_get(rootJ, "discoSpeedQuantity");
    if (discoSpeedQuantityJ)
        discoSpeedQuantity.fromJson(discoSpeedQuantityJ);

    json_t* tagManagerJ = json_object_get(rootJ, "tagManager");
    if (tagManagerJ)
        tagManager.fromJson(tagManagerJ);

    json_t* extraSourcesJ = json_object_get(rootJ, "extraPrefabSources");
    if (extraSourcesJ) {
        extraPrefabSources.clear();
        for (int i = 0; i < json_array_size(extraSourcesJ); i++) {
            json_t* extraSourceJ = json_array_get(extraSourcesJ, i);
            extraPrefabSources.insert(std::make_tuple(json_string_value(json_array_get(extraSourceJ, 0)),
                json_string_value(json_array_get(extraSourceJ, 1))));
        }
    }

    json_t* extraPatchSourcesJ = json_object_get(rootJ, "extraPatchSources");
    if (extraPatchSourcesJ) {
        extraPatchSources.clear();
        for (int i = 0; i < json_array_size(extraPatchSourcesJ); i++) {
            json_t* extraSourceJ = json_array_get(extraPatchSourcesJ, i);
            auto slug = json_string_value(json_array_get(extraSourceJ, 0));
            auto root = json_string_value(json_array_get(extraSourceJ, 1));
            auto pair = std::make_tuple(slug, root);
            extraPatchSources.insert(pair);
        }
    }

    json_t* storageUsernameJ = json_object_get(rootJ, "storageUsername");
    if (storageUsernameJ)
        storageUsername = json_string_value(storageUsernameJ);

    json_t* storagePasswordJ = json_object_get(rootJ, "storagePassword");
    if (storagePasswordJ)
        storagePassword = json_string_value(storagePasswordJ);

    json_t* storageUsersJ = json_object_get(rootJ, "storageUsers");
    if (storageUsersJ) {
        storageUsers.clear();
        for (int i = 0; i < json_array_size(storageUsersJ); i++) {
            json_t* storageUserJ = json_array_get(storageUsersJ, i);
            auto slug = json_string_value(json_array_get(storageUserJ, 0));
            auto id = json_integer_value(json_array_get(storageUserJ, 1));
            auto pair = std::make_tuple(slug, id);
            storageUsers.push_back(pair);
        }
    }
}

void State::reload() {
    store.load();
}

void State::refresh() {
    store.refresh();
}

void State::save() {
    auto path = asset::user("prefabs.json");
    FILE* file = fopen(path.c_str(), "w");
    if (!file) {
        DINFO("[Prefabs] When saving Settings, could not open %s", path.c_str());
        return;
    }

    json_t* rootJ = toJson();
    json_dumpf(rootJ, file, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
    fclose(file);
    json_decref(rootJ);

    DINFO("[Prefabs] Saved Settings to %s", path.c_str());
}

void State::load() {
    auto path = asset::user("prefabs.json");
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        DINFO("[Prefabs] When loading Settings, could not open %s", path.c_str());
        return;
    }

    json_error_t error;
    json_t* rootJ = json_loadf(file, 0, &error);
    if (!rootJ) {
        if (settings::devMode) {
            DINFO("[Prefabs] Failed to load Settings. JSON parsing error at %s %d:%d %s",
                error.source,
                error.line,
                error.column,
                error.text);
        }
        return;
    }

    fromJson(rootJ);
    fclose(file);
    json_decref(rootJ);

    for (auto extraSource : extraPrefabSources) {
        auto slug = std::get<0>(extraSource);
        auto root = std::get<1>(extraSource);
        auto prefabSource = new FileSource(slug, root);
        DINFO("[Prefabs] Adding extra prefab source %s", root.c_str());
        store.addPrefabSource(prefabSource);
    }

    for (auto extraSource : extraPatchSources) {
        auto slug = std::get<0>(extraSource);
        auto root = std::get<1>(extraSource);
        auto patchSource = new ArchiveSource(slug, root);
        DINFO("[Prefabs] Adding extra patch source %s", root.c_str());
        store.addPatchSource(patchSource);
    }

    auto client = StorageClient(storageUsername, storagePassword);
    client.login();

    auto patchSource = new UserStorageSource("patch-storage", asset::user("patch-storage/").c_str());
    store.addStorageSource(patchSource);

    for (auto user : storageUsers) {
        auto slug = std::get<0>(user);
        auto id = std::get<1>(user);

        auto cache = new UserQueryCache(slug, id, client);
        QINFO("[Prefabs] Adding storage source %s", slug.c_str());
        store.addUserQueryCache(cache);
    }

    DINFO("[Prefabs] Loaded Settings from %s", path.c_str());
}
