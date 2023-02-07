#include <ghc/filesystem.hpp>

#include <plugin.hpp>
#include <rack.hpp>

#include "Rack.h"
#include "utils/files.hpp"

namespace fs = ghc::filesystem;

using path = ghc::filesystem::path;

using namespace rack::plugin;

Rack::Rack(std::string source, std::string group, std::string filename)
    : source(source), group(group), filename(filename)
{
    this->name = removeUUID(path(filename).stem().string());
    this->slug = source + "/" + group + "/" + name;
}

void Rack::addPlugin(std::string pluginName, std::string moduleName)
{
    bool found = false;
    for (Plugin* plugin : rack::plugin::plugins) {
        if (plugin->slug == pluginName) {
            for (Model* model : plugin->models) {
                if (model->slug == moduleName) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (!found) {
        this->isValid = false;
        if (this->missingModules.find(pluginName) == this->missingModules.end()) {
            this->missingModules[pluginName] = std::vector<std::string>();
        }
        this->missingModules[pluginName].push_back(moduleName);
    }

    if (this->usedPlugins.find(pluginName) == this->usedPlugins.end()) {
        this->usedPlugins[pluginName] = std::map<std::string, int>();
    }
    if (this->usedPlugins[pluginName].find(moduleName) == this->usedPlugins[pluginName].end()) {
        this->usedPlugins[pluginName][moduleName] = 0;
    }
    this->usedPlugins[pluginName][moduleName]++;
}

std::string Rack::missingReport()
{
    auto reports = std::vector<std::string>();

    for (auto plugin : this->missingModules) {
        auto report = plugin.first + ": ";
        auto names = std::vector<std::string>();

        for (auto module : plugin.second) {
            names.push_back(module);
        }
        report += rack::string::join(names, ", ");
        reports.push_back(report);
    }
    return rack::string::join(reports, "; ");
}

void Rack::fromJson(json_t* rootJ)
{
    json_t* modulesJ = json_object_get(rootJ, "modules");
    if (modulesJ) {
        for (int i = 0; i < json_array_size(modulesJ); i++) {
            json_t* moduleJ = json_array_get(modulesJ, i);
            if (moduleJ) {
                json_t* pluginJ = json_object_get(moduleJ, "plugin");
                if (pluginJ) {
                    std::string plugin = json_string_value(pluginJ);
                    json_t* modelJ = json_object_get(moduleJ, "model");
                    if (modelJ) {
                        std::string model = json_string_value(modelJ);
                        this->addPlugin(plugin, model);
                    }
                }
            }
        }
    }
}

bool operator<(const Rack& lhs, const Rack& rhs)
{
    return lhs.filename < rhs.filename;
}

bool operator>(const Rack& lhs, const Rack& rhs)
{
    return rhs < lhs;
}

bool operator<=(const Rack& lhs, const Rack& rhs)
{
    return !(lhs > rhs);
}

bool operator>=(const Rack& lhs, const Rack& rhs)
{
    return !(lhs < rhs);
}

bool operator==(const Rack& lhs, const Rack& rhs)
{
    return lhs.filename == rhs.filename;
}

bool operator!=(const Rack& lhs, const Rack& rhs)
{
    return !(lhs == rhs);
}