#include "RackTooltip.hpp"
#include "ui/VerticalGroup.hpp"
#include "ui/menus/ModularMenuItem.hpp"
#include "ui/menus/scrolling/ModularItem.hpp"

RackTooltip::RackTooltip(Rack rack) : rack(rack) {

    auto group = new VerticalGroup();

    auto title = new ModularItem();
    title->text = "Used modules:";
    title->disabled = true;
    group->addChild(title);

    for (auto pluginPair : rack.usedPlugins) {
        auto pluginName = pluginPair.first;
        auto modules = pluginPair.second;

        for (auto modulePair : modules) {
            auto moduleName = modulePair.first;
            auto count = modulePair.second;

            auto moduleItem = new ModularItem();
            moduleItem->text = moduleName;
            if (count > 1) {
                moduleItem->text += " (" + std::to_string(count) + ")";
            }
            moduleItem->rightTextCallback = [this, moduleItem, moduleName, pluginName]() {
                auto isMissing = false;
                for (auto pluginPair : this->rack.missingModules) {
                    auto _pluginName = pluginPair.first;
                    if (_pluginName != pluginName) {
                        continue;
                    }
                    auto modules = pluginPair.second;
                    for (auto _moduleName : modules) {
                        if (moduleName == _moduleName) {
                            isMissing = true;
                        }
                    }
                }
                moduleItem->color = isMissing ? nvgRGB(0xff, 0x00, 0x00) : nvgRGB(0xff, 0xff, 0xff);
                return pluginName + (isMissing ? " ⚠" : "");
            };
            moduleItem->disabled = false;
            group->addChild(moduleItem);
        }
    }

    group->step();

    box.size = group->box.size;
    drawCallback = [box = group->box](const DrawArgs& args) {
        bndMenuBackground(args.vg, 0.0, 0.0, box.size.x, box.size.y, BND_CORNER_NONE);
    };
    stepCallback = [this, group]() {
        box.size = group->box.size;
    };
    addChild(group);
}