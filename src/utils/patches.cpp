#include "patches.hpp"

bool isPatchLegacyV1(std::string path)
{
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        return false;
    DEFER({ std::fclose(f); });
    // All Zstandard frames start with this magic number.
    char zstdMagic[] = "\x28\xb5\x2f\xfd";
    char buf[4] = {};
    std::fread(buf, 1, sizeof(buf), f);
    // If the patch file doesn't begin with the magic number, it's a legacy patch.
    return std::memcmp(buf, zstdMagic, sizeof(buf)) != 0;
}