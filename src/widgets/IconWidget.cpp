#include "IconWidget.hpp"
#include <tag.hpp>
#include "menus/IconMenuBuilder.hpp"
#include "menus/ModularMenuItem.hpp"
#include "menus/prefabs/PluginItem.hpp"
#include "plugin.h"
#include "utils/colors.hpp"
#include "utils/mouse.hpp"

SvgWidget* createSvg()
{
    auto bg = new SvgWidget();
    bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/P.svg")));
    bg->box.size = Vec(64, 64);
    return bg;
}

IconWidget::IconWidget()
{
    state = new State();
    state->load();
    state->refresh();
    menuBuilder = new IconMenuBuilder(state);
    disco = DiscoUpdater(state);
    addChild(createSvg());
    box.size = Vec(64, 64);
    box.pos = state->pos;
}

IconWidget::~IconWidget()
{
    state->save();
    delete menuBuilder;
}

void IconWidget::step()
{
    if (visible && !state->showing) {
        hide();
    }
    else if (!visible && state->showing) {
        show();
    }

    if (visible) {
        disco.process();
    }

    OpaqueWidget::step();
}

void IconWidget::draw(const DrawArgs& args)
{
    auto colorValue = state->colorQuantity.getValue();
    auto color = rainbow(state->discoSpeedQuantity.getValue() > 0.0001 ? disco.amount : colorValue);
    nvgBeginPath(args.vg);
    nvgCircle(args.vg, box.size.x / 2 + .5, box.size.y / 2 + .5, 33);
    nvgFillColor(args.vg, color);
    nvgFill(args.vg);

    OpaqueWidget::draw(args);
}
void IconWidget::onButton(const event::Button& e)
{
    e.consume(this);

    if (isRightClick(e)) {
        if (state->refreshEveryTime) {
            state->refresh();
        }
        menuBuilder->build();
    }
}

void IconWidget::onDragMove(const event::DragMove& e)
{
    if (e.button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    state->pos = box.pos = box.pos.plus(e.mouseDelta);
    e.consume(this);
}

void IconWidget::onDragEnd(const DragEndEvent& e)
{
    if (e.button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    state->pos = box.pos;
    state->save();
    e.consume(this);
}