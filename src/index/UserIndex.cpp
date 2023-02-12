#include "UserIndex.hpp"
#include "utils/logging.hpp"

void UserIndex::addRack(Rack rack)
{
    DINFO("UserIndex::addRack(%s)", rack.slug.c_str());
    racks.insert(rack);
    groups[rack.group].insert(rack);

    for (auto [pluginName, modules] : rack.usedPlugins) {
        for (auto [moduleName, count] : modules) {
            plugins[pluginName][moduleName].insert(rack);
        }
    }
}