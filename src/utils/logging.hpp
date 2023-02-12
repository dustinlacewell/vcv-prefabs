#include <rack.hpp>

#define DINFO(format, ...)                            \
    if (rack::settings::devMode)                      \
    rack::logger::log(rack::logger::INFO_LEVEL,       \
        __FILE__,                                     \
        __LINE__,                                     \
        __FUNCTION__,                                 \
        ("[Prefabs]:" + std::string(format)).c_str(), \
        ##__VA_ARGS__)

#define SOURCE_DEBUGGING

#ifdef SOURCE_DEBUGGING
#define SINFO(format, ...)                                        \
    if (rack::settings::devMode)                                  \
    rack::logger::log(rack::logger::INFO_LEVEL,                   \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __FUNCTION__,                                             \
        ("[Prefabs]: (Sources): " + std::string(format)).c_str(), \
        ##__VA_ARGS__)
#else
#define SINFO(format, ...)
#endif

#define CLIENT_DEBUGGING

#ifdef CLIENT_DEBUGGING
#define CINFO(format, ...)                                        \
    if (rack::settings::devMode)                                  \
    rack::logger::log(rack::logger::INFO_LEVEL,                   \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __FUNCTION__,                                             \
        ("[Prefabs]: (Sources): " + std::string(format)).c_str(), \
        ##__VA_ARGS__)
#else
#define SINFO(format, ...)
#endif

#define CACHE_DEBUGGING

#ifdef CACHE_DEBUGGING
#define QINFO(format, ...)                                        \
    if (rack::settings::devMode)                                  \
    rack::logger::log(rack::logger::INFO_LEVEL,                   \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __FUNCTION__,                                             \
        ("[Prefabs]: (Sources): " + std::string(format)).c_str(), \
        ##__VA_ARGS__)
#else
#define SINFO(format, ...)
#endif
