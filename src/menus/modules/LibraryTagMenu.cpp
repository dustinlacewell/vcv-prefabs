#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "LibraryPluginItem.hpp"
#include "LibraryTagItem.hpp"
#include "LibraryTagMenu.hpp"

LibraryTagMenu::LibraryTagMenu(State* state, std::string label, bool favoritesOnly)
{
    this->state = state;
    this->text = label;
    this->favoritesOnly = favoritesOnly;

    this->childMenuCallback = [this, favoritesOnly](ModularMenuItem* item, Menu* indexSubMenu) {
        auto label = new MenuLabel();
        label->text = "Tags";
        indexSubMenu->addChild(label);

        auto hiddenTags = this->state->tagManager.hiddenTags;

        for (int tagId = 0; tagId < (int)tag::tagAliases.size(); tagId++) {
            if (hiddenTags.find(tagId) != hiddenTags.end()) {
                continue;
            }

            auto tagItem = new LibraryTagItem(this->state, tagId, favoritesOnly);
            indexSubMenu->addChild(tagItem);
        }
    };
}