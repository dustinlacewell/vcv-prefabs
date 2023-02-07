#include "files.hpp"
#include <regex>

bool isRegularFile(const std::string& path)
{
    struct stat prefab_sb;
    bool zeroStat = stat(path.c_str(), &prefab_sb) == 0;
    bool isReg = S_ISREG(prefab_sb.st_mode);
    bool isFile = zeroStat && isReg;
    return isFile;
}

bool isDirectory(const std::string& path)
{
    struct stat sb;
    bool zeroStat = stat(path.c_str(), &sb) == 0;
    bool isDir = S_ISDIR(sb.st_mode);
    bool isDirectory = zeroStat && isDir;
    return isDirectory;
}

std::string extensionFrom(const std::string& path)
{
    size_t lastDot = path.find_last_of('.');
    if (lastDot == std::string::npos) {
        return "";
    }
    return path.substr(lastDot + 1);
}

std::string basenameFrom(const std::string& path)
{
    // basename is file without extension
    size_t lastSlash = path.find_last_of('/');
    size_t lastDot = path.find_last_of('.');
    if (lastSlash != std::string::npos) {
        if (lastDot != std::string::npos) {
            return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
        }
        else {
            return path.substr(lastSlash + 1);
        }
    }
    else {
        if (lastDot != std::string::npos) {
            return path.substr(0, lastDot);
        }
        else {
            return path;
        }
    }
}

std::string dirnameFrom(const std::string& path)
{
    // return ONLY the LAST non-filename directory part of a path
    size_t lastSlash = path.find_last_of('/');
    size_t secondLastSlash = path.find_last_of('/', lastSlash - 1);
    if (lastSlash != std::string::npos) {
        if (secondLastSlash != std::string::npos) {
            return path.substr(secondLastSlash + 1, lastSlash - secondLastSlash - 1);
        }
        else {
            return path.substr(0, lastSlash);
        }
    }
    else {
        return "";
    }
}

// dirnameFrom("foo.txt") -> ""
// dirnameFrom("foo/bar.txt") -> "foo"
// dirnameFrom("foo/bar/baz.txt") -> "bar"

void withDir(const std::string& path, std::function<void(DIR*)> fn)
{
    DIR* dir = opendir(path.c_str());
    if (dir) {
        fn(dir);
        closedir(dir);
    }
}

void eachDir(const std::string& path, std::function<void(dirent*)> fn)
{
    withDir(path, [&](auto dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_name[0] == '.')
                continue;

            fn(ent);
        }
    });
}

std::vector<std::string> allFiles(std::string rootPath)
{
    // recursively find all files in a directory
    std::vector<std::string> files;

    eachDir(rootPath, [&](auto ent) {
        std::string path = rootPath + "/" + ent->d_name;
        if (isDirectory(path)) {
            auto subFiles = allFiles(path);
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        }
        else {
            files.push_back(path);
        }
    });

    return files;
}

std::vector<std::string> filesWithExtension(std::vector<std::string>& files, std::string ext)
{
    std::vector<std::string> filtered;
    for (auto& file : files) {
        if (extensionFrom(file) == ext) {
            filtered.push_back(file);
        }
    }
    return filtered;
}

std::string findFirstChild(std::string root, std::string child)
{
    if (child.find(root) != 0) {
        return "";
    }

    std::string result = child.substr(root.length());
    if (result[0] == '/') {
        result.erase(0, 1);
    }

    size_t pos = result.find("/");
    if (pos != std::string::npos) {
        result.erase(pos);
    }
    return result;
}

std::string removeUUID(const std::string& path)
{
    std::regex uuidPattern("-[a-fA-F0-9]{8}");
    return std::regex_replace(path, uuidPattern, "");
}