#pragma once

#include <rack.hpp>
#include "ui/ModularWidget.hpp"

using namespace rack;

struct TabBar : ModularWidget {
    std::vector<std::string> tabNames;
    std::function<void(std::string)> changeCallback;

    int activeTabIndex = 0;
    int hoverTabIndex = -1;

    TabBar(std::vector<std::string> tabNames, std::function<void(std::string)> changeCallback);
    void draw(const DrawArgs& args) override;
    void onButton(const event::Button& e) override;
    float calculateNaturalTabWidth();
    float calculateTabWidth();
    void handleEnter(const event::Enter& e);
    void handleLeave(const event::Leave& e);
    void handleHover(const event::Hover& e);
};