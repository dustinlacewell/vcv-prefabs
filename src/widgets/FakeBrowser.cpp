#include "FakeBrowser.hpp"
#include "IconWidget.hpp"

FakeBrowser::FakeBrowser()
{
    browserBackup = APP->scene->browser;
    browserBackup->hide();
    APP->scene->removeChild(browserBackup);

    APP->scene->browser = this;
    APP->scene->addChild(this);
}

FakeBrowser::~FakeBrowser()
{
    if (APP->scene->browser == this) {
        APP->scene->browser = browserBackup;
        APP->scene->addChild(browserBackup);
        APP->scene->removeChild(this);
    }
}

void FakeBrowser::step()
{
    if (visible) {
        if (!wasVisible) {
            hide();
            // find IconWidget in Scene
            IconWidget* iconWidget = NULL;
            for (Widget* w : APP->scene->children) {
                if (dynamic_cast<IconWidget*>(w)) {
                    iconWidget = dynamic_cast<IconWidget*>(w);
                    break;
                }
            }

            // if IconWidget is not found, abort
            if (!iconWidget) {
                return;
            }

            iconWidget->menuBuilder->build();
        }
    }

    wasVisible = visible;
}

void FakeBrowser::onButton(const event::Button& e)
{
    OpaqueWidget::onButton(e);

    if (e.action == GLFW_PRESS) {
        hide();
        e.consume(this);
    }
}