#include "IconWidget.hpp"
#include <tag.hpp>
#include "menus/IconMenuBuilder.hpp"
#include "menus/ModularMenuItem.hpp"
#include "menus/PluginItem.hpp"
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

math::Rect getRect(math::Rect box)
{
    math::Rect rect;
    rect.pos = APP->window->getSize() / 2 - box.size / 2;
    rect.pos.x = 0;
    rect.size = Vec(64, 64);
    return rect;
}

IconWidget::IconWidget(Prefabs* module)
{
    this->module = module;
    menuBuilder = new IconMenuBuilder(module);
    disco = DiscoUpdater(module);
    addChild(createSvg());
    box = getRect(box);
}

void IconWidget::step()
{
    if (visible && !module->showing) {
        hide();
    }
    else if (!visible && module->showing) {
        show();
    }

    if (visible) {
        disco.process();
    }

    OpaqueWidget::step();
}

void IconWidget::draw(const DrawArgs& args)
{
    auto colorValue = module->colorQuantity.getValue();
    auto color = rainbow(module->discoSpeedQuantity.getValue() > 0.0001 ? disco.amount : colorValue);
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
        menuBuilder->build();
    }
}

void IconWidget::onDragMove(const event::DragMove& e)
{
    if (e.button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    module->pos = box.pos = box.pos.plus(e.mouseDelta);
    e.consume(this);
}