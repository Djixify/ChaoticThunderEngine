#ifndef OBJECTS_TEXTURE
#define OBJECTS_TEXTURE
#include <filesystem>

class Texture {
private:
    unsigned int _texture_ID;
    bool _has_alpha;
public:
    int Width, Height, Channels;
    unsigned char* Data;
    std::filesystem::path Path;

    Texture(std::filesystem::path path, bool has_alpha = false);
    ~Texture();

    void LoadCPU();
    void TransferToGPU(bool destroy_cpu_copy = true);

    void SetActive(int i = 0);

    void FreeCPU();
    void FreeGPU();
};
#endif