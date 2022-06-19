#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

Window::Window(std::string title, int width, int height)
{
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
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
    return nullptr;
}

void Window::SetActive() const
{
    glfwMakeContextCurrent(this->GetGLContext());
}
