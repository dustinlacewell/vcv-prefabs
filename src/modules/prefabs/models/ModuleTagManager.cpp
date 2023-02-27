#include "ModuleTagManager.hpp"
#include <tag.hpp>
#include "ui/menus/ModularMenuItem.hpp"

ModuleTagManager::ModuleTagManager() {
    hiddenTags = std::set<int>();
}

void ModuleTagManager::toggleTag(int tagId) {
    if (hiddenTags.find(tagId) != hiddenTags.end()) {
        hiddenTags.erase(tagId);
    } else {
        hiddenTags.insert(tagId);
    }
}

MenuItem* ModuleTagManager::createToggleMenu() {
    auto item = new ModularMenuItem();
    item->text = "Enabled tags";
    item->rightText = RIGHT_ARROW;
    item->childMenuCallback = [this](ModularMenuItem* item, Menu* subMenu) {
        for (int tagId = 0; tagId < (int)tag::tagAliases.size(); tagId++) {
            auto tag = tag::getTag(tagId);
            auto tagItem = new ModularMenuItem();
            tagItem->text = tag;
            tagItem->rightTextCallback = [this, tagId]() {
                return CHECKMARK(hiddenTags.find(tagId) == hiddenTags.end());
            };
            tagItem->buttonCallback = [this, tagItem, tagId](const event::Button& e) {
                toggleTag(tagId);
                e.consume(tagItem);
                return false;
            };
            subMenu->addChild(tagItem);
        }
    };
    return item;
}

json_t* ModuleTagManager::toJson() {
    auto rootJ = json_object();
    auto hiddenTagsJ = json_array();
    for (auto tagId : hiddenTags) {
        json_array_append_new(hiddenTagsJ, json_integer(tagId));
    }
    json_object_set_new(rootJ, "hiddenTags", hiddenTagsJ);
    return rootJ;
}

void ModuleTagManager::fromJson(json_t* rootJ) {
    auto hiddenTagsJ = json_object_get(rootJ, "hiddenTags");
    if (hiddenTagsJ) {
        for (size_t i = 0; i < json_array_size(hiddenTagsJ); i++) {
            auto tagId = json_integer_value(json_array_get(hiddenTagsJ, i));
            hiddenTags.insert(tagId);
        }
    }
}
