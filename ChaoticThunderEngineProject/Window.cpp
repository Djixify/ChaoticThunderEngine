#include <map>
#include <utility>
#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"
#include "Shader.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Window::Window(std::string title, int width, int height, Window* other) : _activeshader(0), _environment_fill_mesh(true), _global_uniform_time(.0f), _start_time(std::clock()), _delta_time(.0f), _current_time(.0f)
{
    _glfwwindow = glfwCreateWindow(width, height, "LearnOpenGL", NULL, other == NULL ? NULL : other->GetGLContext());
    if (_glfwwindow == NULL)
    {
        Debug::Logger::Console(Debug::Level::WARNING, "Failed to make window instance");
        glfwTerminate();
        Controller::Instance()->ThrowException("Failed to make window instance");
    }

    ClearColor = glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f };

    //Initialize a single camera to be the default view
    Camera* camera = new Camera();
    _cameras.push_back(std::shared_ptr<Camera>(camera));
    _activecamera = 0;

    //Add bindings to events


    //UpdateShaderFiles();
    //window->AddShader("Custom", 2, load_shader{ shader_type::VERTEX, vertex_shaders[selected_vertex_shader] }, load_shader{ shader_type::FRAGMENT, fragment_shaders[selected_fragment_shader] });
    //programs = _glfwwindow->GetShaderLabels();

    //_start_time = std::clock();
}

Window::~Window() {

    _buffers.clear();
    _shaders.clear();
    _cameras.clear();
}

void Window::ProcessKeyChanged(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void Window::ProcessKeyContinuous()                                                            
{
    /*
    Camera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
    if (glfwGetKey(_glfwwindow, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(_glfwwindow, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(_glfwwindow, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(_glfwwindow, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    */
}

void Window::ProcessMousePosition(GLFWwindow* window, double xpos, double ypos)                
{

}

void Window::ProcessMouseKeyChanged(GLFWwindow* window, int button, int action, int mods)      
{

}

void Window::ProcessMouseKeyContinuous()   
{

}

void Window::ProcessMouseEnterLeave(GLFWwindow* window, int entered)                           
{

}

void Window::ProcessScrollWheel(GLFWwindow* window, double xoffset, double yoffset)            
{

}

void Window::ProcessResize(GLFWwindow* window, int width, int height)                          
{
    glViewport(0, 0, width, height);
    Debug::Logger::ConsoleOpenGLError("During setting viewport in window size changed event");
}

void Window::GetSize(int& width, int& height) 
{
    glfwGetFramebufferSize(_glfwwindow, &width, &height);
}

float Window::GetAspectRatio() {
    int width = 0, height = 0;
    GetSize(width, height);
    return (float)width / (float)height;
}


void Window::Clear() {
    int display_w, display_h;
    glfwGetFramebufferSize(_glfwwindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    Debug::Logger::ConsoleOpenGLError("During setting viewport in ClearWindow");
    glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
    Debug::Logger::ConsoleOpenGLError("During setting clear color in ClearWindow");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Debug::Logger::ConsoleOpenGLError("During clear call in ClearWindow");

    glPolygonMode(GL_FRONT_AND_BACK, _environment_fill_mesh ? GL_FILL : GL_LINE);
    Debug::Logger::ConsoleOpenGLError("During setting polygonmode");
}

/// <summary>
/// Determines how the cursor acts when inside the window's context
/// Normal is the default setting when creating a window
/// Hidden applies what it says, acts the same as normal, but hides the cursor
/// Centered is when attaching the camera direction to the mouse so it is bound to the center of the window
/// </summary>
/// <param name="input_type">Type of input that the window cursor should use</param>
void Window::SetCursorInputType(Controls::cursor_input_type input_type) 
{
    glfwSetInputMode(_glfwwindow, GLFW_CURSOR, (int)Controls::cursor_input_type::NORMAL);
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

Camera* Window::GetActiveCamera() {
    return _cameras[_activecamera].get();
}
