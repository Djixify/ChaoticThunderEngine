#ifndef ENGINE_SHADER
#define ENGINE_SHADER

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <map>
#include "Structs.hpp"
#include "Enums.hpp"
#include "Buffer.hpp"

class ArrayBuffer;
class Window;


const char* ReadShaderProgram(const std::string path, bool debug_print = false);

class Shader {
private:
    GLuint _id;
    Window* _window;
    void InitializeProgram(std::vector<load_shader> paths);
public:
    Shader(Window* window, std::vector<load_shader> paths);
    Shader(Window* window, int count, load_shader path...);
    ~Shader();
    unsigned int GetID() const;
    void Use();

    bool SetUniform(std::string name, float value1);
    bool SetUniform(std::string name, float value1, float value2);
    bool SetUniform(std::string name, float value1, float value2, float value3);
    bool SetUniform(std::string name, float value1, float value2, float value3, float value4);
    bool SetUniform(std::string name, int value1);
    bool SetUniform(std::string name, int value1, int value2);
    bool SetUniform(std::string name, int value1, int value2, int value3);
    bool SetUniform(std::string name, int value1, int value2, int value3, int value4);
    bool SetUniform(std::string name, unsigned int value1);
    bool SetUniform(std::string name, unsigned int value1, unsigned int value2);
    bool SetUniform(std::string name, unsigned int value1, unsigned int value2, unsigned int value3);
    bool SetUniform(std::string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4);
};


#endif