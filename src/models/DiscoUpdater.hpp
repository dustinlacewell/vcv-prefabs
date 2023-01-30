#pragma once

#include <rack.hpp>

using namespace rack;

struct DiscoUpdater
{
    Prefabs* module;

    float amount = 0.5f;
    float direction = 1.f;

    dsp::ClockDivider discoDivider;

    DiscoUpdater()
    {
        this->module = nullptr;
        discoDivider.setDivision(2);
    }

    DiscoUpdater(Prefabs* module)
    {
        this->module = module;
        discoDivider.setDivision(2);
    }

    void process()
    {
        if (!module) {
            return;
        }
        if (discoDivider.process()) {
            amount += module->discoSpeedQuantity.getValue() * direction;
            if (amount > 1) {
                amount = 1;
                direction *= -1;
            }
            else if (amount < 0) {
                amount = 0;
                direction *= -1;
            }
        }
    }
};