#ifndef FILE_UTILITY_MODULE
#define FILE_UTILITY_MODULE

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <cstdarg>
#include "Debug.hpp"

namespace File {
    const char pathSeperator = '/'; //May vary between operations systems

    std::string CurrentDirectory();
    std::string GetParent(std::string path);
    std::vector<std::string> GetFiles(std::string path);
    std::vector<std::string> GetDirectories(std::string path);
    std::string CombinePath(int count, std::string args...);
}
#endif