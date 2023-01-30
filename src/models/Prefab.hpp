#pragma once

#include <rack.hpp>

using std::map;
using std::set;
using std::string;

struct Prefab
{
    std::string filename;

    std::set<std::string> tags;
    std::map<std::string, std::map<std::string, int>> usedPlugins;
    std::map<std::string, std::vector<std::string>> missingModules;

    std::string source = "local";

    bool isValid = true;

    Prefab();
    Prefab(std::string filename);
    void addTag(std::string tag);
    void addPlugin(std::string pluginName, std::string moduleName);
    std::string getName() const;
    std::string missingReport();

    void fromJson(json_t* rootJ);
};

bool operator<(const Prefab& lhs, const Prefab& rhs);
bool operator>(const Prefab& lhs, const Prefab& rhs);
bool operator<=(const Prefab& lhs, const Prefab& rhs);
bool operator>=(const Prefab& lhs, const Prefab& rhs);
bool operator==(const Prefab& lhs, const Prefab& rhs);
bool operator!=(const Prefab& lhs, const Prefab& rhs);