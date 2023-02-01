#include <rack.hpp>

using namespace rack;

#define DINFO(format, ...) \
    if (settings::devMode) \
    rack::logger::log(rack::logger::INFO_LEVEL, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
