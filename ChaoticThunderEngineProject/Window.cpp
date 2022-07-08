#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

Window::Window(std::string title, int width, int height, Window* other) : _shader_count(0)
{
    _glfwwindow = glfwCreateWindow(width, height, "LearnOpenGL", NULL, other == NULL ? NULL : other->GetGLContext());
    if (_glfwwindow == NULL)
    {
        Debug::Logger::Console(Debug::Level::WARNING, "Failed to make window instance");
        glfwTerminate();
        Controller::Instance()->ThrowException("Failed to make window instance");
    }
}

void Window::AddShader(std::string shader_name, Shader& shader)
{
    _shaders.insert_or_assign(shader_name, shader);
}

void Window::RemoveShader(std::string shader_name)
{
    std::map<std::string, Shader>::iterator tmp = _shaders.find(shader_name);
    if (tmp != _shaders.end())
        _shaders.erase(tmp);
}

Shader* Window::GetShader(std::string shader_name) {
    std::map<std::string, Shader>::iterator tmp = _shaders.find(shader_name);
    if (tmp != _shaders.end())
        return &_shaders[shader_name];
    else
        return nullptr;
}

GLFWwindow* Window::GetGLContext() const
{
    return _glfwwindow;
}

void Window::SetActive() const
{
    glfwMakeContextCurrent(this->GetGLContext());
}
