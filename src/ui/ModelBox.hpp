#pragma once

#include <rack.hpp>

using namespace rack;

struct ModuleWidgetContainer : widget::Widget
{
    void draw(const DrawArgs& args) override
    {
        Widget::draw(args);
        Widget::drawLayer(args, 1);
    }
};

struct ModelBox : widget::OpaqueWidget
{
    plugin::Model* model;
    ui::Tooltip* tooltip = NULL;
    // Lazily created widgets
    widget::Widget* previewWidget = NULL;
    widget::ZoomWidget* zoomWidget = NULL;
    widget::FramebufferWidget* fb = NULL;
    ModuleWidgetContainer* mwc = NULL;
    ModuleWidget* moduleWidget = NULL;
    float zoom = 1.0f;

    ModelBox();

    void setModel(plugin::Model* model);

    void updateZoom();

    void createPreview();

    void draw(const DrawArgs& args) override;

    void step() override;

    //    void setTooltip(ui::Tooltip* tooltip);

    //    void onButton(const ButtonEvent& e) override;

    void onHoverKey(const HoverKeyEvent& e) override;

    //    ui::Tooltip* createTooltip();

    //    void onEnter(const EnterEvent& e) override;
    //
    //    void onLeave(const LeaveEvent& e) override;

    void onHide(const HideEvent& e) override;

    //    void createContextMenu();
};
