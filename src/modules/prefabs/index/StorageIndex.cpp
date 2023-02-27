#include "StorageIndex.hpp"

void StorageIndex::addRack(Rack rack)
{
    racks.insert(rack);
    users[rack.metadata->author.name].addRack(rack);
}