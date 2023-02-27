#include "SectionHeader.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"
#include "ui/menus/scrolling/helpers.hpp"

SectionHeader::SectionHeader(std::string title, bool showDividers) {
    this->title = title;
    this->showDividers = showDividers;

    auto sep = createSeparator();
    sep->visibleCallback = [this]() {
        return this->showDividers;
    };
    addChild(sep);

    auto label = new ModularItem();
    label->text = title;
    label->disabled = true;
    addChild(label);

    auto sep2 = createSeparator();
    sep2->visibleCallback = [this]() {
        return this->showDividers;
    };
    addChild(sep2);
}