#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <GLFW\glfw3.h>
#include "Shader.hpp"

class Shader;

class Window {
private:
    GLFWwindow* _glfwwindow;
    Shader* _shaders[40];
    int _shader_count;
public:
    Window(std::string title, int width, int height);
    void AddShader(Shader& shader);
    void RemoveShader(int i);
    GLFWwindow* GetGLContext() const;
    void SetActive() const;
};

#endif
