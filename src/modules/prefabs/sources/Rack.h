#pragma once

#include <optional>
#include <rack.hpp>
#include "clients/storage/responses/types/NewPatchInfo.hpp"

using std::map;
using std::set;
using std::string;

struct Rack {
    std::string source;
    std::string group;
    std::string filename;

    // computed
    std::string name;
    std::string slug;
    std::optional<std::string> displayName;
    std::optional<NewPatchInfo> metadata;

    time_t lastModified = 0;

    std::map<std::string, std::map<std::string, int>> usedPlugins;
    std::map<std::string, std::vector<std::string>> missingModules;

    bool isValid = true;

    Rack(std::string source, std::string group, std::string filename);

    void addPlugin(std::string pluginName, std::string moduleName);

    std::string missingReport();

    void fromJson(json_t* rootJ);
    std::string getDisplayName() const;
    std::map<std::string, std::vector<std::string>> getMissingModules();
};

bool operator<(const Rack& lhs, const Rack& rhs);
bool operator>(const Rack& lhs, const Rack& rhs);
bool operator<=(const Rack& lhs, const Rack& rhs);
bool operator>=(const Rack& lhs, const Rack& rhs);
bool operator==(const Rack& lhs, const Rack& rhs);
bool operator!=(const Rack& lhs, const Rack& rhs);