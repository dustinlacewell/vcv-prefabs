#pragma once

#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

#include "modules/prefabs/sources/Rack.h"
#include "ui/VerticalGroup.hpp"

struct RackContextMenu : VerticalGroup {
    Rack rack;

    RackContextMenu(Rack rack);
};