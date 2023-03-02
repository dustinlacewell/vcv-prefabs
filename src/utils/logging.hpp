#pragma once

#include <rack.hpp>

#define CACHE_DEBUGGING
#define SOURCE_DEBUGGING

static inline auto print_helper(auto const& t) {
    return t;
}

static inline auto print_helper(std::string const& s) {
    return s.c_str();
}

template <class... Args>
void dprint(char const* fmt, Args&&... args) {
    if (rack::settings::devMode) {
        printf(fmt, print_helper(args)...);
    }
}

template <typename... Args>
void CINFO(const char* s, const Args&... args) {
#ifdef CACHE_DEBUGGING
    std::string formatStr = "[Prefabs]: (Cache): " + std::string(s) + "\n";
    dprint(formatStr.c_str(), args...);
#endif
}

template <typename... Args>
void DINFO(const char* format, const Args&... args) {
    std::string formatStr = "[Prefabs]: " + std::string(format) + "\n";
    dprint(formatStr.c_str(), args...);
}

template <typename... Args>
void SINFO(const char* format, const Args&... args) {
#ifdef SOURCE_DEBUGGING
    std::string formatStr = "[Prefabs]: (Source): " + std::string(format) + "\n";
    dprint(formatStr.c_str(), args...);
#endif
}

template <typename... Args>
void QINFO(const char* format, const Args&... args) {
#ifdef QUERY_DEBUGGING
    std::string formatStr = "[Prefabs]: (Query): " + std::string(format) + "\n";
    dprint(formatStr.c_str(), args...);
#endif
}