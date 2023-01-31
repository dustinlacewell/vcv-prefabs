#include "PluginSubMenuBuilder.hpp"

MenuItem* buildModuleSubItem(Model* model)
{
    auto moduleItem = new ModularMenuItem();
    moduleItem->text = model->name;

    moduleItem->buttonCallback = [model](const event::Button& e) {
        settings::ModuleInfo& mi = settings::moduleInfos[model->plugin->slug][model->slug];
        mi.added++;
        mi.lastAdded = system::getUnixTime();

        // Record history
        history::ComplexAction* h = new history::ComplexAction;
        h->name = "add module";

        auto newModule = model->createModule();
        APP->engine->addModule(newModule);
        auto widget = model->createModuleWidget(newModule);
        APP->scene->rack->updateModuleOldPositions();
        APP->scene->rack->addModuleAtMouse(widget);

        widget->loadTemplate();

        // Record history
        history::ModuleAdd* ha = new history::ModuleAdd;
        ha->setModule(widget);
        h->push(ha);
        APP->history->push(h);

        e.consume(widget);
        return true;
    };
    moduleItem->rightClickCallback = [model, moduleItem](const event::Button& e) {
        model->setFavorite(!model->isFavorite());
        e.consume(moduleItem);
        return true;
    };
    moduleItem->visibleCallback = [model, moduleItem]() {
        moduleItem->rightText = CHECKMARK(model->isFavorite());
        return true;
    };
    moduleItem->tooltipCallback = [model]() {
        auto modelbox = new ModelBox();
        modelbox->setModel(model);
        modelbox->createPreview();
        modelbox->zoom = 0.8f;
        modelbox->updateZoom();

        std::vector<std::string> tagAliases;

        for (auto& tagId : model->tagIds) {
            for (const std::string& tagAlias : tag::tagAliases[tagId]) {
                tagAliases.push_back(tagAlias);
            }
        }
        std::string tagStr = rack::string::join(tagAliases, ", ");
        std::transform(tagStr.begin(), tagStr.end(), tagStr.begin(), ::tolower);

        auto group = new VerticalGroup();

        group->addChild(modelbox);

        auto tagLabel = new Label();
        group->addChild(tagLabel);
        tagLabel->text = tagStr;
        Rect r;
        modelbox->zoomWidget->getViewport(r);
        tagLabel->box.size.x = fmax(300, r.getWidth());

        return group;
    };

    return moduleItem;
}

std::vector<MenuItem*> createModuleIndexModuleSubMenu(PluginSubMenuData data)
{
    std::vector<MenuItem*> items;
    std::transform(data.modules.begin(), data.modules.end(), std::back_inserter(items), [](Model* model) {
        return buildModuleSubItem(model);
    });
    return items;
};

MenuItem* buildPluginSubMenu(PluginSubMenuData data)
{
    auto item = new ModularMenuItem();
    item->text = data.plugin->slug;
    item->childMenuCallback = [data](ModularMenuItem* item, Menu* pluginSubMenu) {
        for (auto moduleItem : createModuleIndexModuleSubMenu(data)) {
            pluginSubMenu->addChild(moduleItem);
        }
    };
    return item;
}