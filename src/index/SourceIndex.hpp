#pragma once

#include <rack.hpp>
#include "sources/Rack.h"

using namespace rack;

typedef std::set<Rack> RackSet;
typedef std::map<std::string, RackSet> RackGroups;
typedef std::map<std::string, RackGroups> PluginRacks;

struct SourceIndex
{
    RackSet racks;
    RackGroups groups;
    PluginRacks plugins;

    void addRack(Rack rack);
};