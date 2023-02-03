#include "PrefabStore.hpp"
#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

PrefabStore::PrefabStore()
{
    sources = std::map<std::string, PrefabSource>();
    sources.emplace("local", PrefabSource("local", asset::user("selections")));

    for (auto plugin : plugin::plugins) {
        addSource(PrefabSource(plugin->slug, asset::plugin(plugin, "res/selections")));
    }
}

void PrefabStore::refresh()
{
    total = 0;
    for (auto& [slug, source] : sources) {
        source.refresh();
        sources.emplace(slug, source);
        total += source.total;
    }
}

void PrefabStore::addSource(PrefabSource source)
{
    sources.emplace(source.slug, source);
}

int PrefabStore::getTotal()
{
    return total;
}

PrefabSource& PrefabStore::getLocalSource()
{
    return sources["local"];
}