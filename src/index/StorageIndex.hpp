#pragma once

#include <rack.hpp>
#include "SourceIndex.hpp"
#include "UserIndex.hpp"
#include "sources/Rack.h"

using namespace rack;

struct StorageIndex {
    RackSet racks;

    std::map<std::string, UserIndex> users;

    void addRack(Rack rack);
};