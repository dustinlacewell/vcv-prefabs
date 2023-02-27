#include <tag.hpp>

#include "LibraryResultItem.hpp"
#include "ui/ModelBox.hpp"

LibraryResultItem::LibraryResultItem(State* state, ModuleIndex* modules, int index)
{
    this->state = state;
    this->modules = modules;
    this->index = index;

    this->visibleCallback = [this]() {
        auto size = this->modules->results.size();
        auto maxSize = this->state->searchResultsQuantity.getValue();
        auto limit = fmin(size, maxSize);

        if (this->index < limit) {
            auto model = this->modules->results[this->index];
            if (this->text != model->name) {
                this->text = model->name;
                this->rightText = model->plugin->name;

                if (model->isFavorite()) {
                    this->rightText += CHECKMARK(true);
                }

                auto modelbox = new ModelBox();
                modelbox->setModel(model);
                modelbox->zoom = 0.8f;
                modelbox->updateZoom();

                this->setTooltip(modelbox);
            }
            return true;
        }
        return false;
    };
    this->buttonCallback = [state, modules, index](const event::Button& e) {
        auto size = modules->results.size();
        auto maxSize = state->searchResultsQuantity.getValue();
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
        h->name = "add state";

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
}