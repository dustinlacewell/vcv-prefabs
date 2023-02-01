#include "PrefabStore.hpp"
#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

PrefabStore::PrefabStore()
{
    sources = std::map<std::string, PrefabSource>();
    sources.emplace("local", PrefabSource(nullptr));

    for (auto plugin : plugin::plugins) {
        addSource(PrefabSource(plugin));
    }
}

void PrefabStore::refresh()
{
    for (auto& [slug, source] : sources) {
        source.refresh();
        sources.emplace(slug, source);
    }
}

void PrefabStore::addSource(PrefabSource source)
{
    sources.emplace(source.plugin->slug, source);
}

int PrefabStore::total()
{
    int total = 0;
    for (auto& [slug, source] : sources) {
        total += source.total;
    }
    return total;
}

PrefabSource& PrefabStore::getLocalSource()
{
    return sources["local"];
}