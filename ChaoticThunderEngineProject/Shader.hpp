#ifndef ENGINE_SHADER
#define ENGINE_SHADER

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <unordered_map>
#include "Window.hpp"
#include "Structs.hpp"
#include "Enums.hpp"
#include "Buffer.hpp"

class ArrayBuffer;
class Window;

struct load_shader {
    shader_type type;
    std::string path;
};


const char* ReadShaderProgram(const std::string path, bool debug_print = false);

class Shader {
private:
    GLuint _id;
    Window& _window;
    std::unordered_map<std::string, ArrayBuffer*> _buffermap;
public:
    Shader(Window& window, int count, load_shader path...);
    ~Shader();
    unsigned int GetID() const;
    void Use();
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformInteger(const std::string& name, int value);
    ArrayBuffer* AddArrayBuffer(const std::string& label);
    ArrayBuffer* AddArrayBuffer(const char* label);
    ArrayBuffer* GetArrayBuffer(const std::string& label);
    ArrayBuffer* GetArrayBuffer(const char* label);
    void RemoveArrayBuffer(const std::string& label);
    void RemoveArrayBuffer(const char* label);
};


#endif