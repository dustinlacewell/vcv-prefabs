#pragma once

#include <rack.hpp>

#include "LightLatch.hpp"
#include "Prefabs.hpp"

struct WidgetToggle : LightLatch
{
    Prefabs* module;

    WidgetToggle(Prefabs* module);
};