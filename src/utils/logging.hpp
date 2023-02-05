#include <rack.hpp>

using namespace rack;

#define DINFO(format, ...)                            \
    if (settings::devMode)                            \
    rack::logger::log(rack::logger::INFO_LEVEL,       \
        __FILE__,                                     \
        __LINE__,                                     \
        __FUNCTION__,                                 \
        ("[Prefabs]:" + std::string(format)).c_str(), \
        ##__VA_ARGS__)

#define SOURCE_DEBUGGING

#ifdef SOURCE_DEBUGGING
#define SINFO(format, ...)                                        \
    if (settings::devMode)                                        \
    rack::logger::log(rack::logger::INFO_LEVEL,                   \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __FUNCTION__,                                             \
        ("[Prefabs]: (Sources): " + std::string(format)).c_str(), \
        ##__VA_ARGS__)
#else
#define SINFO(format, ...)
#endif
