#include "SourceIndex.hpp"
#include "utils/logging.hpp"

void SourceIndex::addRack(Rack rack)
{
    DINFO("SourceIndex::addRack(%s)", rack.slug.c_str());
    racks.insert(rack);
    groups[rack.group].insert(rack);

    for (auto [pluginName, modules] : rack.usedPlugins) {
        for (auto [moduleName, count] : modules) {
            plugins[pluginName][moduleName].insert(rack);
        }
    }
}