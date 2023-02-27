#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularMenuLabel : MenuLabel
{
    std::function<bool()> visibleCallback;

    void step() override;
};