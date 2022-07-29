#include "File.hpp"
#include <filesystem>

namespace File {
    std::string CurrentDirectory() {
        return std::filesystem::current_path().generic_u8string();
    }

    std::string GetParent(std::string path) {
        size_t index = path.find_last_of(pathSeperator);
        return path.substr(0, index);
    }


    std::vector<std::string> GetFiles(std::string path) {
        std::vector<std::string> files;
        std::filesystem::directory_iterator directoryiter(path);
        for (std::filesystem::directory_entry entry : directoryiter) {
            if (entry.is_regular_file()) {
                std::wstring name = entry.path().native();
                std::string str_path = std::string(name.begin(), name.end());
                files.push_back(str_path);
            }
        }
        return files;
    }

    std::vector<std::string> GetDirectories(std::string path) {
        std::vector<std::string> dirs;
        std::filesystem::directory_iterator directoryiter(path);
        for (std::filesystem::directory_entry entry : directoryiter) {
            if (entry.is_directory()) {
                std::wstring name = entry.path().native();
                std::string str_path = std::string(name.begin(), name.end());
                dirs.push_back(str_path);
            }
        }
        return dirs;
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