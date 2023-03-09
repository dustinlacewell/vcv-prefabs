#include "ScrollableItem.hpp"
#include "utils/logging.hpp"

ScrollableItem::ScrollableItem() {
    box.size.y = 24;
    box.size.x = bndLabelWidth(APP->window->vg, -1, text.c_str());
}

void ScrollableItem::draw(const DrawArgs& args) {
    BNDwidgetState state = BND_DEFAULT;

    if (APP->event->hoveredWidget == this)
        state = BND_HOVER;

    // Set active state if this ScrollableItem
    ScrollableMenu* parentMenu = getAncestorOfType<ScrollableMenu>();
    if (parentMenu && parentMenu->activeEntry == this)
        state = BND_ACTIVE;

    // Main text and background
    if (!disabled) {
        //        bndMenuItem(args.vg, 0.0, 0.0, box.size.x, box.size.y, state, -1, text.c_str());
        // replicate bndMenuItem but do it manually so we can use the text color
        if (state != BND_DEFAULT) {
            auto theme = bndGetTheme();
            bndInnerBox(args.vg,
                0,
                0,
                box.size.x,
                box.size.y,
                0,
                0,
                0,
                0,
                bndOffsetColor(theme->menuItemTheme.innerSelectedColor, theme->menuItemTheme.shadeTop),
                bndOffsetColor(theme->menuItemTheme.innerSelectedColor, theme->menuItemTheme.shadeDown));
            bndIconLabelValue(
                args.vg, 0, 0, box.size.x, box.size.y, -1, color, BND_LEFT, BND_LABEL_FONT_SIZE, text.c_str(), NULL);
            state = BND_ACTIVE;
        }
        bndIconLabelValue(
            args.vg, 0.0, 0.0, box.size.x, box.size.y, -1, color, BND_LEFT, BND_LABEL_FONT_SIZE, text.c_str(), NULL);
    }

    else
        bndMenuLabel(args.vg, 0.0, 0.0, box.size.x, box.size.y, -1, text.c_str());

    // Right text
    float x = box.size.x - bndLabelWidth(args.vg, -1, rightText.c_str());
    NVGcolor rightColor = (state == BND_DEFAULT && !disabled) ? bndGetTheme()->menuTheme.textColor
                                                              : bndGetTheme()->menuTheme.textSelectedColor;
    bndIconLabelValue(args.vg,
        x,
        0.0,
        box.size.x,
        box.size.y,
        -1,
        rightColor,
        BND_LEFT,
        BND_LABEL_FONT_SIZE,
        rightText.c_str(),
        NULL);
}

void ScrollableItem::step() {
    // Add 10 more pixels because measurements on high-DPI screens are sometimes too small for some reason
    const float rightPadding = 10.0;
    // HACK use APP->window->vg from the window.
    // All this does is inspect the font, so it shouldn't modify APP->window->vg and should work when called from a widget::FramebufferWidget for example.
    box.size.x = bndLabelWidth(APP->window->vg, -1, text.c_str()) + rightPadding;
    if (!rightText.empty())
        box.size.x += bndLabelWidth(APP->window->vg, -1, rightText.c_str()) - 10.0;

    if (!visible) {
        box.size.x = 0.0;
    }
    Widget::step();
}

void ScrollableItem::onEnter(const EnterEvent& e) {
    // find ScrollableMenu ancestor
    //    auto parentMenu = dynamic_cast<ScrollableMenu*>(parent->parent->parent);
    auto parentMenu = getAncestorOfType<ScrollableMenu>();
    if (!parentMenu)
        return;

    if (parentMenu->activeEntry == this) {
        return;
    }

    parentMenu->setChildMenu(NULL);

    // Try to create child menu
    ScrollableMenu* childMenu = createChildMenu();
    if (childMenu) {
        parentMenu->activeEntry = this;
        childMenu->box.pos = this->box.pos;

        // for every parent until MenuOverlay
        // keep adding the parent's position to the childMenu's position
        for (Widget* w = parent; w; w = w->parent) {
            if (w->box.pos.isFinite()) {
                childMenu->box.pos = childMenu->box.pos.plus(w->box.pos);
            }
            if (dynamic_cast<MenuOverlay*>(w)) {
                break;
            }
        }

        parentMenu->setChildMenu(childMenu);

        // find the most ancestral ScrollableMenu
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        ScrollableMenu* ancestorMenu = parentMenu->getAncestorOfType<ScrollableMenu>();

        if (ancestorMenu) {
            childMenu->box.pos.x = APP->scene->getAbsoluteOffset(ancestorMenu->box.getTopRight()).x;
        } else {
            ScrollableMenu* siblingMenu = NULL;
            bool skipped = false;
            // find the second to last ScrollableMenu child of overlay
            // go in reverse order using rbegin() and rend()
            for (auto it = overlay->children.rbegin(); it != overlay->children.rend(); it++) {
                if (dynamic_cast<ScrollableMenu*>(*it)) {
                    if (skipped) {
                        siblingMenu = dynamic_cast<ScrollableMenu*>(*it);
                        break;
                    }
                    skipped = true;
                }
            }

            if (siblingMenu) {
                childMenu->box.pos.x = APP->scene->getAbsoluteOffset(siblingMenu->box.getTopRight()).x;
            } else {
                childMenu->box.pos.x += this->box.size.x;
            }
        }

        childMenu->box.size.x = std::max(childMenu->box.size.x, 40.f);
        childMenu->box.size.y = std::max(childMenu->box.size.y, 40.f);
    }
}

void ScrollableItem::onDragDrop(const DragDropEvent& e) {
    if (e.origin == this && !disabled) {
        int mods = APP->window->getMods();
        doAction((mods & RACK_MOD_MASK) != RACK_MOD_CTRL);
    }
}

void ScrollableItem::doAction(bool consume) {
    widget::EventContext cAction;
    ActionEvent eAction;
    eAction.context = &cAction;
    if (consume) {
        eAction.consume(this);
    }
    onAction(eAction);
    if (!cAction.consumed)
        return;

    // Close menu
    MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
    if (overlay) {
        overlay->requestDelete();
    }
}

void ScrollableItem::onAction(const ActionEvent& e) {}