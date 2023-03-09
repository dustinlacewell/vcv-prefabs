#pragma once

#include <rack.hpp>

#define CACHE_DEBUGGING
#define SOURCE_DEBUGGING

static inline void log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

template <typename... Args>
void CINFO(const char* s, Args... args) {
#ifdef CACHE_DEBUGGING
    std::string formatStr = "[Prefabs]: (Cache): " + std::string(s) + "\n";
    log(formatStr.c_str(), args...);
#endif
}

template <typename... Args>
void DINFO(const char* format, Args... args) {
    std::string formatStr = "[Prefabs]: " + std::string(format) + "\n";
    log(formatStr.c_str(), args...);
}

template <typename... Args>
void SINFO(const char* format, Args... args) {
#ifdef SOURCE_DEBUGGING
    std::string formatStr = "[Prefabs]: (Source): " + std::string(format) + "\n";
    log(formatStr.c_str(), args...);
#endif
}

template <typename... Args>
void QINFO(const char* format, Args... args) {
#ifdef QUERY_DEBUGGING
    std::string formatStr = "[Prefabs]: (Query): " + std::string(format) + "\n";
    dprint(formatStr.c_str(), args...);
#endif
}