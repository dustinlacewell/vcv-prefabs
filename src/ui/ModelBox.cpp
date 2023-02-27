#include "ModelBox.hpp"

ModelBox::ModelBox()
{
    this->zoom = 0.01f;
    updateZoom();
}

void ModelBox::setModel(plugin::Model* model)
{
    this->model = model;
}

void ModelBox::updateZoom()
{
    float zoom = this->zoom;

    if (previewWidget) {
        fb->setDirty();
        zoomWidget->setZoom(zoom);
        box.size.x = moduleWidget->box.size.x * zoom;
    }
    else {
        // Approximate size as 12HP before we know the actual size.
        // We need a nonzero size, otherwise too many ModelBoxes will lazily render in the same frame.
        box.size.x = 12 * RACK_GRID_WIDTH * zoom;
    }
    box.size.y = RACK_GRID_HEIGHT * zoom;
    box.size = box.size.ceil();
}

void ModelBox::createPreview()
{
    if (previewWidget)
        return;

    previewWidget = new widget::TransparentWidget;
    addChild(previewWidget);

    zoomWidget = new widget::ZoomWidget;
    previewWidget->addChild(zoomWidget);

    fb = new widget::FramebufferWidget;
    if (APP->window->pixelRatio < 2.0) {
        // Small details draw poorly at low DPI, so oversample when drawing to the framebuffer
        fb->oversample = 2.0;
    }
    zoomWidget->addChild(fb);

    mwc = new ModuleWidgetContainer;
    fb->addChild(mwc);

    moduleWidget = model->createModuleWidget(NULL);
    mwc->addChild(moduleWidget);
    mwc->box.size = moduleWidget->box.size;

    updateZoom();
}

void ModelBox::draw(const DrawArgs& args)
{
    // Lazily create preview when drawn
    createPreview();

    // To avoid ModelBox::blinding the user when rack brightness is low, draw framebuffer with the same brightness.
    float b = math::clamp(settings::rackBrightness + 0.2f, 0.f, 1.f);
    nvgGlobalTint(args.vg, nvgRGBAf(b, b, b, 1));

    Widget::draw(args);
}

void ModelBox::step()
{
    Widget::step();
    box = box.nudge(APP->scene->rack->box.zeroPos());
}

void ModelBox::onHoverKey(const HoverKeyEvent& e)
{
    if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT) {
        if (e.key == GLFW_KEY_F1 && (e.mods & RACK_MOD_MASK) == RACK_MOD_CTRL) {
            system::openBrowser(model->getManualUrl());
            e.consume(this);
        }
    }

    if (e.isConsumed())
        return;
    Widget::onHoverKey(e);
}

void ModelBox::onHide(const HideEvent& e)
{
    Widget::onHide(e);
}