#include "plugin.h"

Plugin* pluginInstance;

__attribute__((__visibility__("default"))) void init(Plugin* p)
{
    pluginInstance = p;
    p->addModel(modelPrefabs);
}
