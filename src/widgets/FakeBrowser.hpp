#pragma once

#include <rack.hpp>

using namespace rack;

struct FakeBrowser : OpaqueWidget
{
    bool wasVisible = false;
    Widget* browserBackup;

    FakeBrowser();
    ~FakeBrowser();

    void step() override;
    void onButton(const event::Button& e) override;
};