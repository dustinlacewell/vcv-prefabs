#pragma once

#include <rack.hpp>
#include "ui/VerticalGroup.hpp"

using namespace rack;

struct SectionHeader : VerticalGroup {
    std::string title;
    bool showDividers;

    SectionHeader(std::string title, bool showDividers);
};