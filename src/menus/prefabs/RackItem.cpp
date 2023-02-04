#include "RackItem.hpp"
#include <patch.hpp>
#include "utils/patches.hpp"

RackItem::RackItem(State* state, Rack newRack) : rack(newRack)
{
    this->state = state;
    this->text = newRack.name;
    this->rightText = "";
    this->source = "";
    this->rack = newRack;
    this->tooltip = new Tooltip;
    APP->scene->addChild(tooltip);
    tooltip->text = "Missing modules:\n" + newRack.missingReport();
    tooltip->hide();
}

void RackItem::onButton(const event::Button& e)
{
    e.consume(this);
    if (this->disabled) {
        return;
    }

    // get rack.filename's extension
    std::string extension = rack.filename.substr(rack.filename.find_last_of(".") + 1);

    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        // if it's a .vcv file, unarchive it first
        if (extension == "vcv" && !isPatchLegacyV1(rack.filename)) {
            auto patchRoot = asset::user("rack-tmp");
            auto patchPath = patchRoot + '/' + "patch.json";

            system::createDirectories(patchRoot);
            system::unarchiveToDirectory(rack.filename, patchRoot);
            APP->scene->rack->loadSelection(patchPath);
        }
        else {
            APP->scene->rack->loadSelection(rack.filename);
        }

        for (auto mw : APP->scene->rack->getSelected()) {
            e.consume(mw);
        }
    }
    else if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        // create a context menu with a single item to load the patch
        if (extension != "vcv") {
            return;
        }

        Menu* menu = createMenu();
        auto item = new ModularMenuItem();
        item->text = "Load patch";
        item->buttonCallback = [this](const event::Button& e) {
            MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
            if (overlay) {
                overlay->requestDelete();
            }
            APP->patch->load(rack.filename);
            return true;
        };
        menu->addChild(item);
        return;
    }

    // Close menu
    MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
    if (overlay) {
        overlay->requestDelete();
    }
}

void RackItem::onHover(const event::Hover& e)
{
    e.consume(this);
}

void RackItem::onEnter(const event::Enter& e)
{
    e.consume(this);
    if (this->disabled) {
        tooltip->show();
    }
}

void RackItem::onLeave(const event::Leave& e)
{
    e.consume(this);
    tooltip->hide();
}

void RackItem::step()
{
    // if the control key is pressed
    if ((APP->window->getMods() & RACK_MOD_MASK) == RACK_MOD_CTRL) {
        text = rack.filename;
    }
    else {
        text = rack.name;
    }

    rightText = rack.isValid ? source : source + "!";
    this->disabled = !rack.isValid;
    tooltip->box.pos = APP->scene->mousePos;

    ModularMenuItem::step();
}

void RackItem::draw(const DrawArgs& args)
{
    MenuItem::draw(args);
    //    ModularMenuItem::draw(args);
    //
    //    if (!this->disabled) {
    //        if (tooltip->visible) {
    //            tooltip->hide();
    //        }
    //        return;
    //    }
}