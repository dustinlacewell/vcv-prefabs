#include "PatchStore.hpp"
#include <plugin.hpp>
#include <rack.hpp>

using namespace rack;

PatchStore::PatchStore()
{
    sources = std::map<std::string, PatchSource>();
    sources.emplace("local", PatchSource("local", asset::user("patches")));

    for (auto plugin : plugin::plugins) {
        addSource(PatchSource(plugin->slug, asset::plugin(plugin, "res/patches")));
    }
}

void PatchStore::refresh()
{
    for (auto& [slug, source] : sources) {
        source.refresh();
        sources.emplace(slug, source);
    }
}

void PatchStore::addSource(PatchSource source)
{
    sources.emplace(source.slug, source);
}

int PatchStore::total()
{
    int total = 0;
    for (auto& [slug, source] : sources) {
        total += source.total;
    }
    return total;
}

PatchSource& PatchStore::getLocalSource()
{
    return sources["local"];
}