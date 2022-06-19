#ifndef FILE_UTILITY_MODULE
#define FILE_UTILITY_MODULE

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <cstdarg>
#include "Debug.hpp"

namespace FileUtility {
    const char pathSeperator = '/'; //May vary between operations systems

    std::string CurrentDirectory() {
        return std::filesystem::current_path().generic_u8string();
    }

    std::string GetParent(std::string path) {
        size_t index = path.find_last_of(pathSeperator);
        return path.substr(0, index);
    }

    std::string CombinePath(int count, std::string args...) {
        va_list folders;
        std::string path = "";

        va_start(folders, count);
        for (int i = 0; i < count; i++)
            path = i == 0 ? va_arg(folders, std::string) : path.append(&pathSeperator).append(va_arg(folders, std::string));
        va_end(folders);

        return path;
    }
}
#endif