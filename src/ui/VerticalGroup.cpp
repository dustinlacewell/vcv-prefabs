#include "VerticalGroup.hpp"

void VerticalGroup::step()
{
    ModularWidget::step();

    auto totalHeight = 0;
    for (widget::Widget* child : children) {
        if (!child->visible)
            continue;
        totalHeight += child->box.size.y;
    }

    // if our width is not infinite, then we need to set the width of all children to our width
    if (box.size.x != INFINITY) {
        for (auto child : children) {
            if (!child->visible)
                continue;
            child->box.size.x = std::max(box.size.x, child->box.size.x);
        }
    }

    // Set positions of children
    box.size = math::Vec(0, 0);

    for (auto child : children) {
        if (!child->visible)
            continue;

        child->box.pos = math::Vec(0, box.size.y);

        if (child->box.size.x > box.size.x) {
            box.size.x = child->box.size.x;
        }

        box.size.y += child->box.size.y;
    }
}