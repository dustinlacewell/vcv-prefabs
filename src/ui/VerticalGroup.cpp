#include "VerticalGroup.hpp"

void VerticalGroup::step()
{
    Widget::step();

    // Set positions of children
    box.size = math::Vec(0, 0);
    for (auto child : children) {
        if (!child->visible)
            continue;
        // Increment height, set position of child
        child->box.pos = math::Vec(0, box.size.y);
        box.size.y += child->box.size.y;
        // Increase width based on maximum width of child
        if (child->box.size.x > box.size.x) {
            box.size.x = child->box.size.x;
        }
    }

    // Set widths of all children to maximum width
    for (widget::Widget* child : children) {
        child->box.size.x = box.size.x;
    }
}