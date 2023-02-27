#pragma once

#include <rack.hpp>
#include "modules/prefabs/sources/Rack.h"

typedef std::set<Rack> RackSet;
typedef std::map<std::string, RackSet> RackGroups;
typedef std::map<std::string, RackGroups> PluginRacks;
