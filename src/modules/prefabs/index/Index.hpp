#pragma once

#include <rack.hpp>
#include "SourceIndex.hpp"

using namespace rack;

struct Index {
    RackSet racks;

    std::map<std::string, SourceIndex> sources;

    void addRack(Rack rack);
};