#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <GLFW\glfw3.h>
#include "Shader.hpp"

class Shader;

class Window {
private:
    GLFWwindow* _glfwwindow;
    std::map<std::string, Shader*> _shaders;
    int _shader_count;
public:
    Window(std::string title, int width, int height, Window* other = nullptr);
    void AddShader(std::string shader_name, Shader* shader);
    void RemoveShader(std::string shader_name);
    Shader* GetShader(std::string shader_name);
    GLFWwindow* GetGLContext() const;
    void SetActive() const;
};

#endif
