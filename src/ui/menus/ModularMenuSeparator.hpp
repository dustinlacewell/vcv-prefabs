#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularMenuSeparator : MenuSeparator
{
    std::function<bool()> visibleCallback;

    void step() override;
};