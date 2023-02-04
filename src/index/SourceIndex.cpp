#include "SourceIndex.hpp"

void SourceIndex::addRack(Rack rack)
{
    racks.insert(rack);
    groups[rack.group].insert(rack);

    for (auto [pluginName, modules] : rack.usedPlugins) {
        for (auto [moduleName, count] : modules) {
            plugins[pluginName][moduleName].insert(rack);
        }
    }
}