#pragma once

#include <rack.hpp>

#include "sources/Rack.h"

typedef std::set<Rack> RackSet;
typedef std::map<std::string, RackSet> RackGroups;
typedef std::map<std::string, RackGroups> PluginRacks;
