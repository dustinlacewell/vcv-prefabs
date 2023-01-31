#include "ModuleResultsBuilder.hpp"

MenuItem* buildModuleSearchResult(Prefabs* module, ModuleIndex* modules, int index)
{
    auto item = new ModularMenuItem();
    item->visibleCallback = [module, modules, item, index]() {
        auto size = modules->results.size();
        auto maxSize = module->searchResultsQuantity.getValue();
        auto limit = fmin(size, maxSize);

        if (index < limit) {
            auto model = modules->results[index];
            if (item->text != model->name) {
                item->text = model->name;
                item->rightText = model->plugin->name;

                auto modelbox = new ModelBox();
                modelbox->setModel(model);
                modelbox->zoom = 0.8f;
                modelbox->updateZoom();

                auto tooltip = new Tooltip();
                tooltip->addChild(modelbox);
                tooltip->box.size = modelbox->box.size;

                item->setTooltip(tooltip);
            }
            return true;
        }
        return false;
    };
    item->buttonCallback = [module, modules, index](const event::Button& e) {
        auto size = modules->results.size();
        auto maxSize = module->searchResultsQuantity.getValue();
        auto limit = fmin(size, maxSize);

        if (index >= limit) {
            return false;
        }

        auto model = modules->results[index];

        // Record usage
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
        h->push(APP->scene->rack->getModuleDragAction());
        widget->loadTemplate();

        // Record history
        history::ModuleAdd* ha = new history::ModuleAdd;
        ha->setModule(widget);
        h->push(ha);
        APP->history->push(h);

        e.consume(widget);
        return true;
    };

    return item;
}