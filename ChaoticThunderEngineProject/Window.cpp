#include <map>
#include <utility>
#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"
#include "Shader.hpp"

Window::Window(std::string title, int width, int height, Window* other)
{
    _glfwwindow = glfwCreateWindow(width, height, "LearnOpenGL", NULL, other == NULL ? NULL : other->GetGLContext());
    if (_glfwwindow == NULL)
    {
        Debug::Logger::Console(Debug::Level::WARNING, "Failed to make window instance");
        glfwTerminate();
        Controller::Instance()->ThrowException("Failed to make window instance");
    }
}

Window::~Window() {

    _buffers.clear();
    _shaders.clear();
}

void Window::AddShader(std::string shader_name, int count, load_shader shaders...)
{
    std::vector<load_shader> paths;

    //Read each shader and compile them
    va_list folders;
    va_start(folders, count);
    for (int i = 0; i < count; i++)
        paths.push_back(va_arg(folders, load_shader));
    va_end(folders);

    _shaders.insert_or_assign(shader_name, std::make_shared<Shader>(this, paths));
}

void Window::RemoveShader(std::string shader_name)
{
    if (_shaders.find(shader_name) != _shaders.end())
        _shaders.erase(shader_name);
}

Shader* Window::GetShader(std::string shader_name) {
    return _shaders.find(shader_name) != _shaders.end() ? _shaders[shader_name].get() : nullptr;
}

std::vector<std::string> Window::GetShaderLabels() {
    std::vector<std::string> keys;
    for (std::map<std::string, std::shared_ptr<Shader>>::iterator pair = _shaders.begin(); pair != _shaders.end(); pair++)
        keys.push_back(pair->first);
    return keys;
}

GLFWwindow* Window::GetGLContext() const
{
    return _glfwwindow;
}

void Window::SetActive() const
{
    glfwMakeContextCurrent(this->GetGLContext());
}

ArrayBuffer* Window::AddArrayBuffer(std::string label)
{
    this->_buffers.insert_or_assign(label, std::make_shared<ArrayBuffer>(this));
    return this->_buffers[label].get();
}

ArrayBuffer* Window::GetArrayBuffer(std::string label)
{
    if (this->_buffers.find(label) == this->_buffers.end())
        return nullptr;
    return this->_buffers[label].get();
}

void Window::RemoveArrayBuffer(std::string label) {
    if (this->_buffers.find(label) != this->_buffers.end()) {
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed label %s in shader", label);
        _buffers.erase(label);
    }
    else
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Could not find array buffer by label %s in shader", label);
}