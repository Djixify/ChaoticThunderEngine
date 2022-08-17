#ifndef FILE_UTILITY_MODULE
#define FILE_UTILITY_MODULE

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <cstdarg>
#include "Debug.hpp"
#include "Structs.hpp"

namespace File {
    std::vector<std::filesystem::path> GetFiles(std::filesystem::path path);
    std::vector<std::filesystem::path> GetDirectories(std::filesystem::path path);
    image* LoadPNG(std::filesystem::path path, bool has_alpha = true);
}
#endif