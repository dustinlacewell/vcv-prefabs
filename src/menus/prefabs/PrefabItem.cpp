#include "PrefabItem.hpp"
#include <patch.hpp>
#include "utils/patches.hpp"

PrefabItem::PrefabItem(Prefabs* module, Prefab prefab)
{
    this->module = module;
    this->text = prefab.getName();
    this->rightText = RIGHT_ARROW;
    this->prefab = prefab;
    this->tooltip = new Tooltip;
    APP->scene->addChild(tooltip);
    tooltip->text = "Missing modules:\n" + prefab.missingReport();
    tooltip->hide();
}

void PrefabItem::onButton(const event::Button& e)
{
    e.consume(this);
    if (this->disabled) {
        return;
    }

    // get prefab.filename's extension
    std::string extension = prefab.filename.substr(prefab.filename.find_last_of(".") + 1);

    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        auto rack = APP->scene->rack;

        // if it's a .vcv file, unarchive it first
        if (extension == "vcv" && !isPatchLegacyV1(prefab.filename)) {
            auto patchRoot = asset::user("prefab-tmp");
            auto patchPath = patchRoot + '/' + "patch.json";

            system::createDirectories(patchRoot);
            system::unarchiveToDirectory(prefab.filename, patchRoot);
            rack->loadSelection(patchPath);
        }
        else {
            rack->loadSelection(prefab.filename);
        }

        for (auto mw : rack->getSelected()) {
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
            APP->patch->load(prefab.filename);
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

void PrefabItem::onHover(const event::Hover& e)
{
    e.consume(this);
}

void PrefabItem::onEnter(const event::Enter& e)
{
    e.consume(this);
    if (this->disabled) {
        tooltip->show();
    }
}

void PrefabItem::onLeave(const event::Leave& e)
{
    e.consume(this);
    tooltip->hide();
}

void PrefabItem::step()
{
    // if the control key is pressed
    if ((APP->window->getMods() & RACK_MOD_MASK) == RACK_MOD_CTRL) {
        text = prefab.filename;
    }
    else {
        text = prefab.getName();
    }

    rightText = prefab.isValid ? "" : "!";
    this->disabled = !prefab.isValid;
    tooltip->box.pos = APP->scene->mousePos;

    ModularMenuItem::step();
}

void PrefabItem::draw(const DrawArgs& args)
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