#include <tag.hpp>

#include "LibraryModuleItem.hpp"
#include "ui/ModelBox.hpp"
#include "ui/VerticalGroup.hpp"

LibraryModuleItem::LibraryModuleItem(Model* model)
{
    this->model = model;
    this->text = model->name;

    this->buttonCallback = [model](const event::Button& e) {
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

        widget->loadTemplate();

        // Record history
        history::ModuleAdd* ha = new history::ModuleAdd;
        ha->setModule(widget);
        h->push(ha);
        APP->history->push(h);

        e.consume(widget);
        return true;
    };

    this->rightClickCallback = [model, this](const event::Button& e) {
        model->setFavorite(!model->isFavorite());
        e.consume(this);
        return true;
    };

    this->rightTextCallback = [model]() {
        return model->isFavorite() ? "\u2665" : "";
    };

    this->tooltipCallback = [model]() {
        auto modelbox = new ModelBox();
        modelbox->setModel(model);
        modelbox->createPreview();
        modelbox->zoom = 0.8f;
        modelbox->updateZoom();

        std::vector<std::string> tagAliases;

        for (auto& tagId : model->tagIds) {
            for (const std::string& tagAlias : tag::tagAliases[tagId]) {
                tagAliases.push_back(tagAlias);
                break;
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

        group->box.size = group->getChildrenBoundingBox().size;

        return group;
    };
}