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
    menuBuilder = new IconMenuBuilder(state);
    disco = DiscoUpdater(state);
    addChild(createSvg());
    box.size = Vec(64, 64);
    box.pos = state->pos;
}

IconWidget::~IconWidget()
{
    state->save();
    delete state;
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

        // non-blocking attempt to acquire prefabsMutex
        if (prefabsMutex.try_lock()) {
            // if we have prefabs, add them to the menu
            while (!prefabsQueue.empty()) {
                auto rack = prefabsQueue.front();
                prefabsQueue.pop();
                state->prefabsIndex.addRack(rack);
            }
            prefabsMutex.unlock();
        }

        // non-blocking attempt to acquire patchesMutex
        if (patchesMutex.try_lock()) {
            // if we have patches, add them to the menu
            while (!patchesQueue.empty()) {
                auto rack = patchesQueue.front();
                patchesQueue.pop();
                state->patchesIndex.addRack(rack);
            }
            patchesMutex.unlock();
        }
    }

    OpaqueWidget::step();
}

void IconWidget::draw(const DrawArgs& args)
{
    auto colorValue = state->colorQuantity.getValue();
    auto color = rainbow(state->discoSpeedQuantity.getValue() > 0.0001 ? disco.amount : colorValue);
    nvgBeginPath(args.vg);
    nvgCircle(args.vg, box.size.x / 2 + .5, box.size.y / 2 + .5, 33);
    if (state->monoIcon) {
        // use colorValue to determine brightness
        auto brightness = colorValue * 255;
        nvgFillColor(args.vg, nvgRGB(brightness, brightness, brightness));
    }
    else {
        nvgFillColor(args.vg, color);
    }
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