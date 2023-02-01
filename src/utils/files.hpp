#include <dirent.h>
#include <sys/stat.h>
#include <rack.hpp>

bool isRegularFile(const std::string& path);

bool isDirectory(const std::string& path);

std::string extensionFrom(const std::string& path);

void withDir(const std::string& path, std::function<void(DIR*)> fn);

void eachDir(const std::string& path, std::function<void(dirent*)> fn);
