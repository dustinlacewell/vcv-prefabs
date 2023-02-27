#include "ScrollableMenu.hpp"
#include <ui/Menu.hpp>

ScrollableMenu::ScrollableMenu()
{
    box.size = math::Vec(0, 0);

    list = new List();
    container->addChild(list);
}

void ScrollableMenu::addMenuItem(Widget* child)
{
    list->addChild(child);
}

ScrollableMenu::~ScrollableMenu()
{
    setChildMenu(NULL);
}

void ScrollableMenu::setChildMenu(ScrollableMenu* menu)
{
    if (childMenu) {
        childMenu->parent->removeChild(childMenu);
        delete childMenu;
        childMenu = NULL;
    }

    // for all this->list->children, set their childMenu to NULL
    for (widget::Widget* child : list->children) {
        auto scrollableMenu = dynamic_cast<ScrollableMenu*>(child);
        if (scrollableMenu) {
            scrollableMenu->setChildMenu(NULL);
        }
    }

    if (menu) {
        childMenu = menu;
        // find MenuOverlay
        MenuOverlay* menuOverlay = getAncestorOfType<MenuOverlay>();
        assert(menuOverlay);
        menuOverlay->addChild(childMenu);
    }
}

bool ScrollableMenu::isEmbedded()
{
    auto parentScrollableMenu = getAncestorOfType<ScrollableMenu>();
    return parentScrollableMenu != NULL;
}

void ScrollableMenu::step()
{
    for (widget::Widget* child : list->children) {
        child->step();
    }

    // calculate total height of children
    auto totalHeight = 0;
    for (widget::Widget* child : list->children) {
        if (!child->visible)
            continue;
        totalHeight += child->box.size.y;
    }

    auto margin = maxHeight.has_value() && totalHeight > maxHeight ? 15 : 0;
    auto maxWidth = box.size.x - margin;

    if (isEmbedded()) {
        // embeded menus expand children if we're bigger
        for (auto child : list->children) {
            if (!child->visible)
                continue;

            child->box.size.x = std::max(maxWidth, child->box.size.x);
        }
    }

    // shrink children if they're bigger
    box.size = math::Vec(0, 0);

    for (widget::Widget* child : list->children) {

        if (!child->visible)
            continue;

        if (child->box.size.x + margin > box.size.x) {
            box.size.x = child->box.size.x + margin;
        }

        box.size.y += child->box.size.y;
    }

    list->box.size.x = box.size.x - margin;

    if (maxHeight.has_value() && box.size.y > *maxHeight) {
        box.size.y = std::min(box.size.y, *maxHeight);
    }

    ScrollWidget::step();

    assert(parent);
    box = box.nudge(parent->box.zeroPos());

    visible = box.size.y != 0;
}

void ScrollableMenu::draw(const DrawArgs& args)
{
    if (!isEmbedded()) {
        bndMenuBackground(args.vg, 0.0, 0.0, box.size.x, box.size.y, cornerFlags);
    }
    ScrollWidget::draw(args);
}
