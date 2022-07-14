#ifndef ENGINE_SHADER
#define ENGINE_SHADER

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <map>
#include <ext.hpp>
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
    friend class Window;
public:
    Shader(Window* window, std::vector<load_shader> paths);
    Shader(Window* window, int count, load_shader path...);
    ~Shader();
    unsigned int GetID() const;
    void Use(bool update_global_uniforms = true);
#pragma region Uniforms
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

    bool SetUniform(std::string name, glm::vec1 values);
    bool SetUniform(std::string name, glm::vec2 values);
    bool SetUniform(std::string name, glm::vec3 values);
    bool SetUniform(std::string name, glm::vec4 values);
    bool SetUniform(std::string name, glm::ivec1 values);
    bool SetUniform(std::string name, glm::ivec2 values);
    bool SetUniform(std::string name, glm::ivec3 values);
    bool SetUniform(std::string name, glm::ivec4 values);
    bool SetUniform(std::string name, glm::uvec1 values);
    bool SetUniform(std::string name, glm::uvec2 values);
    bool SetUniform(std::string name, glm::uvec3 values);
    bool SetUniform(std::string name, glm::uvec4 values);

    bool SetUniform(std::string name, glm::mat2x2 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat2x3 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat2x4 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat3x2 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat3x3 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat3x4 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat4x2 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat4x3 values, bool transpose = GL_FALSE);
    bool SetUniform(std::string name, glm::mat4x4 values, bool transpose = GL_FALSE);

    bool SetUniform(std::string name, int count, const float* values);
    bool SetUniform(std::string name, int count, const int* values);
    bool SetUniform(std::string name, int count, const unsigned int* values);
#pragma endregion
};


#endif