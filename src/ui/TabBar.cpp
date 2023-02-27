#include "TabBar.hpp"

#include "ui/menus/scrolling/ScrollableMenu.hpp"
#include "utils/logging.hpp"

/**
* A horizontal tab bar widget for VCV Rack
 *
 * Splits the width of the widget into equal parts for each tab
 *
 * Each tab is a button that can be clicked to change the active tab
 *
 * If the widget's parent's width is not infinite, then the tabs will be sized to fit the parent's width
 * Otherwise each tab will be sized to fit the text
 *
 */

TabBar::TabBar(std::vector<std::string> tabNames, std::function<void(std::string)> changeCallback) {
    this->tabNames = tabNames;
    this->changeCallback = changeCallback;
    box.size.x = calculateNaturalTabWidth() * 3.0f;
    box.size.y = BND_WIDGET_HEIGHT;

    // assign using std::bind
    this->enterCallback = std::bind(&TabBar::handleEnter, this, std::placeholders::_1);
    this->leaveCallback = std::bind(&TabBar::handleLeave, this, std::placeholders::_1);
    this->hoverCallback = std::bind(&TabBar::handleHover, this, std::placeholders::_1);
}

float TabBar::calculateNaturalTabWidth() {
    float tabWidth = 0;

    for (auto tabName : tabNames) {
        auto width = bndLabelWidth(APP->window->vg, -1, tabName.c_str());
        tabWidth = std::max(tabWidth, width);
    }

    return tabWidth;
}

float TabBar::calculateTabWidth() {
    float parentWidth = parent ? parent->box.size.x : INFINITY;

    if (parentWidth != INFINITY && parentWidth > 0.0) {
        return parentWidth / tabNames.size();
    } else {
        // calculate the width of the widest tab
        return calculateNaturalTabWidth();
    }
}

void TabBar::draw(const DrawArgs& args) {
    // if our parent's width is not infinite, then we need to calculate the width of each tab
    float tabWidth = calculateTabWidth();

    float lastWidth = 0;
    // draw the tabs as if they were menu items
    for (int i = 0; i < tabNames.size(); i++) {
        auto tabName = tabNames[i];
        auto x = i * tabWidth;
        auto y = 0;
        auto width = tabWidth;
        auto height = box.size.y;
        auto state = BND_DEFAULT;
        if (i == activeTabIndex) {
            state = BND_ACTIVE;
        }

        // draw the label for the tab, centered
        auto labelWidth = bndLabelWidth(APP->window->vg, -1, tabName.c_str());
        auto labelX = x + (tabWidth / 2);
        auto labelY = y + (height / 2);
        bndMenuItem(args.vg, x, y, width, height, state, -1, "");

        // use raw nanosvg to draw the text
        NVGcolor blue = nvgRGB(0x00, 0x7f, 0xff);
        NVGcolor textColor = hoverTabIndex == i && state != BND_ACTIVE ? blue : nvgRGB(0xff, 0xff, 0xff);
        nvgFontSize(args.vg, 13);
        nvgFontFaceId(args.vg, APP->window->uiFont->handle);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(args.vg, textColor);
        nvgText(args.vg, labelX, labelY, tabName.c_str(), NULL);
    }
}

void TabBar::onButton(const event::Button& e) {
    if (e.action == GLFW_PRESS) {
        // find the tab that was clicked
        float tabWidth = calculateTabWidth();
        auto tab = e.pos.x / tabWidth;
        if (tab >= 0 && tab < tabNames.size()) {
            activeTabIndex = tab;
            changeCallback(tabNames[tab]);
        }
    }

    ModularWidget::onButton(e);
}

void TabBar::handleHover(const event::Hover& e) {
    // calculate which tab the mouse is over
    float tabWidth = calculateTabWidth();
    auto tab = e.pos.x / tabWidth;
    if (tab >= 0 && tab < tabNames.size()) {
        hoverTabIndex = tab;
    } else {
        hoverTabIndex = -1;
    }
}

void TabBar::handleEnter(const event::Enter& e) {
    DINFO("enterCallback");
    auto parentMenu = getAncestorOfType<ScrollableMenu>();
    if (!parentMenu) {
        return;
    }

    parentMenu->setChildMenu(NULL);
}

void TabBar::handleLeave(const event::Leave& e) {
    hoverTabIndex = -1;
}