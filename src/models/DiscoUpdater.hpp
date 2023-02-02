#pragma once

#include <rack.hpp>

using namespace rack;

struct DiscoUpdater
{
    State* state;

    float amount = 0.5f;
    float direction = 1.f;

    dsp::ClockDivider discoDivider;

    DiscoUpdater()
    {
        this->state = nullptr;
        discoDivider.setDivision(2);
    }

    DiscoUpdater(State* state)
    {
        this->state = state;
        discoDivider.setDivision(2);
    }

    void process()
    {
        if (!state) {
            return;
        }
        if (discoDivider.process()) {
            amount += state->discoSpeedQuantity.getValue() * direction;
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