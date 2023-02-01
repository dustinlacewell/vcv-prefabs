#include "PrefabItem.hpp"

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
    if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
        auto rack = APP->scene->rack;

        // get prefab.filename's extension
        std::string extension = prefab.filename.substr(prefab.filename.find_last_of(".") + 1);

        // if it's a .vcv file, unarchive it first
        if (extension == "vcv") {
            auto patchRoot = asset::user("prefab-tmp");
            system::createDirectories(patchRoot);
            system::unarchiveToDirectory(prefab.filename, patchRoot);
            auto patchPath = patchRoot + '/' + "patch.json";
            rack->loadSelection(patchPath);
        }
        else {
            rack->loadSelection(prefab.filename);
        }

        for (auto mw : rack->getSelected()) {
            e.consume(mw);
        }
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