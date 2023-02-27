#include "FakeBrowser.hpp"
#include "IconWidget.hpp"

FakeBrowser::FakeBrowser()
{
    enable();
}

FakeBrowser::~FakeBrowser()
{
    disable();
    Widget::~Widget();
}

void FakeBrowser::enable()
{
    if (APP->scene->browser != this) {
        browserBackup = APP->scene->browser;
        browserBackup->hide();

        APP->scene->browser = this;
        APP->scene->addChild(this);
        hide();
    }
}

// factor out the disable steps above to a static function here
// and call it from the destructor and the destructor of IconWidget
// to avoid code duplication
void FakeBrowser::disable()
{
    if (APP->scene->browser == this) {
        APP->scene->browser = browserBackup;
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