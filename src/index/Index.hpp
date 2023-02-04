#pragma once

#include <rack.hpp>
#include "sources/Rack.h"

using namespace rack;

typedef std::set<Rack> RackSet;
typedef std::map<std::string, RackSet> RackGroup;
typedef std::map<std::string, RackGroup> PluginRacks;

struct Index
{
    RackSet racks;
    RackGroup groups;
    PluginRacks plugins;

    void addRack(Rack rack);
};