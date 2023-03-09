#pragma once

#include <rack.hpp>
#include "modules/prefabs/sources/Rack.h"
#include "ui/ModularWidget.hpp"

using namespace rack;

struct RackTooltip : ModularWidget {

    Rack rack;

    RackTooltip(Rack rack);
};