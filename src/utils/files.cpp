#include "files.hpp"

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
    return path.substr(path.find_last_of(".") + 1);
}

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
