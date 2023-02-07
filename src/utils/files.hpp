#include <dirent.h>
#include <sys/stat.h>
#include <rack.hpp>

bool isRegularFile(const std::string& path);

bool isDirectory(const std::string& path);

std::string extensionFrom(const std::string& path);

void withDir(const std::string& path, std::function<void(DIR*)> fn);

void eachDir(const std::string& path, std::function<void(dirent*)> fn);

std::vector<std::string> allFiles(const std::string rootPath);

std::vector<std::string> filesWithExtension(std::vector<std::string>& files, std::string ext);

std::string basenameFrom(const std::string& path);

std::string dirnameFrom(const std::string& path);

std::string findFirstChild(std::string root, std::string path);

std::string removeUUID(const std::string& path);