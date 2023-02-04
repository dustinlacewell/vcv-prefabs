#pragma once

#include <rack.hpp>
#include "SourceIndex.hpp"
#include "sources/Rack.h"

using namespace rack;

typedef std::set<Rack> RackSet;
typedef std::map<std::string, RackSet> RackGroups;
typedef std::map<std::string, RackGroups> PluginRacks;

struct Index
{
    RackSet racks;

    std::map<std::string, SourceIndex> sources;

    void addRack(Rack rack);
};