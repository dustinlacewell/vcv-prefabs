#include "Latch.hpp"

Latch::Latch()
{
    fb = new widget::FramebufferWidget;
    addChild(fb);

    shadow = new CircularShadow;
    fb->addChild(shadow);
    shadow->box.size = math::Vec();

    sw = new widget::SvgWidget;
    fb->addChild(sw);

    addFrame(Svg::load(asset::system("res/ComponentLibrary/VCVButton_0.svg")));
    addFrame(Svg::load(asset::system("res/ComponentLibrary/VCVButton_1.svg")));
}

void Latch::step()
{
    if (isLatched) {
        latched = isLatched();
    }

    ModularWidget::step();
}

void Latch::onButton(const event::Button& e)
{
    if (e.button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (e.action == GLFW_PRESS) {
        if (frames.size() >= 2) {
            sw->setSvg(frames[1]);
            fb->setDirty();
        }
    }

    if (e.action == GLFW_RELEASE) {
        if (frames.size() >= 1) {
            sw->setSvg(frames[0]);
            fb->setDirty();
        }
        latched = !latched;
        if (latched) {
            if (latchCallback)
                latchCallback();
        }
        else {
            if (unlatchCallback)
                unlatchCallback();
        }
    }

    ModularWidget::onButton(e);
}

void Latch::addFrame(std::shared_ptr<window::Svg> svg)
{
    frames.push_back(svg);
    // If this is our first frame, automatically set SVG and size
    if (!sw->svg) {
        sw->setSvg(svg);
        box.size = sw->box.size;
        fb->box.size = sw->box.size;
        // Move shadow downward by 10%
        shadow->box.size = sw->box.size;
        shadow->box.pos = math::Vec(0, sw->box.size.y * 0.10);
    }
}