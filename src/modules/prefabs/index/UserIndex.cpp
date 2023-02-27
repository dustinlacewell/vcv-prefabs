#include "UserIndex.hpp"
#include "utils/logging.hpp"

void UserIndex::addRack(Rack rack)
{
    if (racks.find(rack) != racks.end()) {
        DINFO("UserIndex::addRack(%s) already exists", rack.slug.c_str());
        return;
    }

    DINFO("UserIndex::addRack(%s)", rack.slug.c_str());
    racks.insert(rack);

    sortedRacks.push_back(rack);
    std::sort(
        sortedRacks.begin(), sortedRacks.end(), [](Rack a, Rack b) { return a.metadata->title < b.metadata->title; });

    for (auto [pluginName, modules] : rack.usedPlugins) {
        for (auto [moduleName, count] : modules) {
            plugins[pluginName][moduleName].insert(rack);
        }
    }
}