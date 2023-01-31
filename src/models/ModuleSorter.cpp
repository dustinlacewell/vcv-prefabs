#include "ModuleSorter.hpp"
#include "menus/ModularMenuItem.hpp"

int getModuleUsage(Model* model)
{
    settings::ModuleInfo& mi = settings::moduleInfos[model->plugin->slug][model->slug];
    return mi.added;
}

ModuleSorter::ModuleSorter()
{
    sortType = FAVORITE_THEN_ALPHA;
}

std::vector<Model*> ModuleSorter::sort(std::vector<Model*> modules)
{
    switch (sortType) {
        case ALPHA:
            std::sort(modules.begin(), modules.end(), [](Model* a, Model* b) { return a->name < b->name; });
            break;
        case USAGE:
            std::sort(modules.begin(), modules.end(), [](Model* a, Model* b) {
                return getModuleUsage(a) > getModuleUsage(b);
            });
            break;
        case FAVORITE_THEN_ALPHA:
            std::sort(modules.begin(), modules.end(), [](Model* a, Model* b) {
                if (a->isFavorite() == b->isFavorite()) {
                    return a->name < b->name;
                }
                return a->isFavorite() > b->isFavorite();
            });
            break;
        case FAVORITE_THEN_USAGE:
            std::sort(modules.begin(), modules.end(), [](Model* a, Model* b) {
                // sort by alpha in the case of usage tie
                if (a->isFavorite() == b->isFavorite()) {
                    if (getModuleUsage(a) == getModuleUsage(b)) {
                        return a->name < b->name;
                    }
                    return getModuleUsage(a) > getModuleUsage(b);
                }
                return a->isFavorite() > b->isFavorite();
            });
            break;
    }
    return modules;
}

MenuItem* ModuleSorter::createSortTypeMenu()
{
    auto item = new ModularMenuItem();
    item->text = "Sort modules by";
    item->childMenuCallback = [this](ModularMenuItem* item, Menu* menu) {
        auto alphaItem = new ModularMenuItem();
        alphaItem->text = "Alphabetical";
        alphaItem->rightText = CHECKMARK(sortType == ALPHA);
        alphaItem->buttonCallback = [this, alphaItem](const event::Button& e) {
            sortType = ALPHA;
            e.consume(alphaItem);
            return false;
        };
        alphaItem->visibleCallback = [this, alphaItem]() {
            alphaItem->rightText = CHECKMARK(sortType == ALPHA);
            return true;
        };
        menu->addChild(alphaItem);

        auto usageItem = new ModularMenuItem();
        usageItem->text = "Usage";
        usageItem->rightText = CHECKMARK(sortType == USAGE);
        usageItem->buttonCallback = [this, usageItem](const event::Button& e) {
            sortType = USAGE;
            e.consume(usageItem);
            return false;
        };
        usageItem->visibleCallback = [this, usageItem]() {
            usageItem->rightText = CHECKMARK(sortType == USAGE);
            return true;
        };
        menu->addChild(usageItem);

        auto favoriteThenAlphaItem = new ModularMenuItem();
        favoriteThenAlphaItem->text = "Favorite then alphabetical";
        favoriteThenAlphaItem->rightText = CHECKMARK(sortType == FAVORITE_THEN_ALPHA);
        favoriteThenAlphaItem->buttonCallback = [this, favoriteThenAlphaItem](const event::Button& e) {
            sortType = FAVORITE_THEN_ALPHA;
            e.consume(favoriteThenAlphaItem);
            return false;
        };
        favoriteThenAlphaItem->visibleCallback = [this, favoriteThenAlphaItem]() {
            favoriteThenAlphaItem->rightText = CHECKMARK(sortType == FAVORITE_THEN_ALPHA);
            return true;
        };
        menu->addChild(favoriteThenAlphaItem);

        auto favoriteThenUsageItem = new ModularMenuItem();
        favoriteThenUsageItem->text = "Favorite then usage";
        favoriteThenUsageItem->rightText = CHECKMARK(sortType == FAVORITE_THEN_USAGE);
        favoriteThenUsageItem->buttonCallback = [this, favoriteThenUsageItem](const event::Button& e) {
            sortType = FAVORITE_THEN_USAGE;
            e.consume(favoriteThenUsageItem);
            return false;
        };
        favoriteThenUsageItem->visibleCallback = [this, favoriteThenUsageItem]() {
            favoriteThenUsageItem->rightText = CHECKMARK(sortType == FAVORITE_THEN_USAGE);
            return true;
        };
        menu->addChild(favoriteThenUsageItem);
    };

    return item;
}

json_t* ModuleSorter::toJson()
{
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "sortType", json_integer(sortType));
    return rootJ;
}

void ModuleSorter::fromJson(json_t* rootJ)
{
    json_t* sortTypeJ = json_object_get(rootJ, "sortType");
    if (sortTypeJ) {
        sortType = (SortType)json_integer_value(sortTypeJ);
    }
}
