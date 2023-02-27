#pragma once

#include <rack.hpp>
#include "ModularWidget.hpp"

using namespace rack;

struct VerticalGroup : ModularWidget {
    void step() override;
};