#pragma once

#include <rack.hpp>
#include "sources/Rack.h"
#include "typedefs.hpp"

using namespace rack;

struct UserIndex {
    RackSet racks;        // all racks
    RackGroups groups;    // racks by patch-source patch
    PluginRacks plugins;  // racks

    void addRack(Rack rack);
};