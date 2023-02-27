#pragma once

#include <rack.hpp>
#include "typedefs.hpp"

using namespace rack;

struct UserIndex {
    RackSet racks;  // all racks
    std::vector<Rack> sortedRacks;
    PluginRacks plugins;  // racks

    void addRack(Rack rack);
};