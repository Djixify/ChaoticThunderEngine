#include <filesystem>
#ifndef OBJECTS_TEXTURE
#define OBJECTS_TEXTURE

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

    void SetActive();

    void FreeCPU();
    void FreeGPU();
};
#endif