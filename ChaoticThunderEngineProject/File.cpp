#include "File.hpp"
#include <filesystem>

namespace File {

    std::vector<std::filesystem::path> GetFiles(std::filesystem::path path) {
        std::vector<std::filesystem::path> files;
        std::filesystem::directory_iterator directoryiter(path);
        for (std::filesystem::directory_entry entry : directoryiter) {
            if (entry.is_regular_file())
                files.push_back(entry.path());
        }
        return files;
    }

    std::vector<std::filesystem::path> GetDirectories(std::filesystem::path path) {
        std::vector<std::filesystem::path> dirs;
        std::filesystem::directory_iterator directoryiter(path);
        for (std::filesystem::directory_entry entry : directoryiter) {
            if (entry.is_directory())
                dirs.push_back(entry.path());
        }
        return dirs;
    }
}