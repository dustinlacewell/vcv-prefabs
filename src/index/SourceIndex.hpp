#pragma once

#include <rack.hpp>
#include "sources/Rack.h"
#include "typedefs.hpp"

using namespace rack;

struct SourceIndex
{
    RackSet racks;
    RackGroups groups;
    PluginRacks plugins;

    void addRack(Rack rack);
};