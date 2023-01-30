#include "plugin.h"

Plugin* pluginInstance;

void init(Plugin* p)
{
    pluginInstance = p;
    p->addModel(modelPrefabs);
}
