#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

Window::Window(std::string title, int width, int height) : _shader_count(0), _shaders{0}
{
    _glfwwindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (_glfwwindow == NULL)
    {
        Debug::Logger::Console(Debug::Level::WARNING, "Failed to make window instance");
        glfwTerminate();
        Controller::Instance()->ThrowException("Failed to make window instance");
    }
}

void Window::AddShader(Shader& shader)
{
}

void Window::RemoveShader(int i)
{
}

GLFWwindow* Window::GetGLContext() const
{
    return _glfwwindow;
}

void Window::SetActive() const
{
    glfwMakeContextCurrent(this->GetGLContext());
}
