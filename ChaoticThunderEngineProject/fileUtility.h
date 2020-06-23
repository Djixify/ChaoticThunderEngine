#pragma once
#include <string>
#include <windows.h>
namespace FileUtility {
    const char pathSeperator = '/'; //May vary between operations systems

    std::string currentDirectory() {
        return std::filesystem::current_path().generic_u8string();
    }

    std::string getParent(std::string path) {
        size_t index = path.find_last_of(pathSeperator);
        return path.substr(0, index);
    }

    std::string combinePath(std::string path1, std::string path2) {
        return path1.append(&pathSeperator).append(path2);
    }
}
