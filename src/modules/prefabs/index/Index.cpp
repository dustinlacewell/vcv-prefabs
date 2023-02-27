#include "Index.hpp"

void Index::addRack(Rack rack)
{
    racks.insert(rack);
    sources[rack.source].addRack(rack);
}