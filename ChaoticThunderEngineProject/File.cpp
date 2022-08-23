#include "File.hpp"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Structs.hpp"

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

    image* LoadPNG(std::filesystem::path path, bool has_alpha) {
        image* result = new image();
        result->data = stbi_load(path.generic_string().c_str(), &result->width, &result->height, &result->channels, has_alpha ? 4 : 3);
        return result;
    }
}