
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Debug.hpp"
#include "Controller.hpp"
#include "Window.hpp"

void _defaultProcessFocus(GLFWwindow* window, int gl_message) {
    if (gl_message == GL_TRUE) {
        for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
            if (Controller::Instance()->GetWindow(i)->GetGLContext() == window) {
                Controller::Instance()->SetContextWindow(i);
                std::cout << "Context window set to " << i << std::endl;
            }
        }
    }
    else {
        //Do stuff when losing focus to lower resources...
    }
}

void _defaultProcessResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Debug::Logger::ConsoleOpenGLError("During attempt to resize in event");
}

Controller* Controller::_instance = nullptr;

void Controller::Init() {
    if (_instance == nullptr) {
        _instance = new Controller();


        //Initialize GLFW
        if (!glfwInit())
            _instance->ThrowException("Could not initialize controller as GLFW failed to initialize");
        //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _instance->_windows = (Window**)malloc(sizeof(Window*) * _instance->_window_capacity);
    }
}

Controller* Controller::Instance() {
    return _instance;
}

int Controller::GetWindowCount() {
    return _window_count;
}

int Controller::GetWindowCapacity() {
    return _window_capacity;
}

Window* Controller::GetWindow(int i) const {
    if (i >= _window_count || i < 0)
        Controller::Instance()->ThrowException("Window index out of bounds");
    return _windows[i];
}

int Controller::GetWindowIndex(const Window* window) {
    for (int i = 0; i < _window_count; i++)
        if (_windows[i] == window)
            return i;
    return -1;
}

Window* Controller::FirstWindow() const {
    return GetWindow(0);
}

Window* Controller::LastWindow() const {
    return GetWindow(_window_count - 1);
}

Window* Controller::GetContextWindow() const {
    return GetWindow(_context_window);
}

int Controller::GetContextWindowIndex() {
    return _context_window;
}

void Controller::SetContextWindow(int i) {
    if (i >= _window_capacity || i < 0)
        Controller::Instance()->ThrowException("Window index out of bounds");

    _windows[i]->SetActive();
    //Load glad only works AFTER setting a context window
    if (!_hasloadedGLAD) {
        gladLoadGL();
        _hasloadedGLAD = true;
    }

    _context_window = i;
}

void Controller::ClearContextWindow() {
    glfwMakeContextCurrent(GL_NONE);
    _context_window = -1;
}

void Controller::AddWindow(Window* w) {
    if (_window_count == _window_capacity)
        this->ThrowException("Out of window capacity, use SetCapacity(i) to set a larger capacity");

    _windows[_window_count] = w;

    if (_context_window < 0)
        this->SetContextWindow(_window_count);

    glfwSetWindowFocusCallback(_windows[_window_count]->GetGLContext(), _defaultProcessFocus);
    glfwSetFramebufferSizeCallback(_windows[_window_count]->GetGLContext(), _defaultProcessResize);

    _window_count++;
}

void Controller::SetWindowCapacity(int cap) {
    if (cap < 0)
        this->ThrowException("Invalid capacity given");

    _window_capacity = cap;

    int oldCount = _window_count;

    if (_window_count > _window_capacity)
        _window_count = _window_capacity;

    if (_context_window >= _window_capacity)
        SetContextWindow(_window_count - 1);

    //Make new array 
    Window** newArr = (Window**)malloc(sizeof(Window*) * _window_capacity);
    int i;
    for (i = 0; i < std::min(_window_count, _window_capacity); i++) {
        newArr[i] = _windows[i];
    }

    //Dispose extra windows
    for (; i < oldCount; i++) {
        glfwDestroyWindow(_windows[i]->GetGLContext());
    }
    free(_windows);

    //Set to new array
    _windows = newArr;
}

void Controller::RemoveWindow(int i) {
    if (i >= _window_count || i < 0)
        Controller::Instance()->ThrowException("Window index out of bounds");

    //See if the context window needs to get updated
    if (_context_window >= i) {
        _context_window--;
        if (_context_window < 0 && _window_count > 1)
            Controller::Instance()->SetContextWindow(i == 0);
    }

    glfwDestroyWindow(_windows[i]->GetGLContext());

    //Update windows array
    _window_count--;
    for (int j = i; j < _window_count; j++) {
        _windows[j] = _windows[j + 1];
    }

}

int Controller::GetMaxNumberOfVertexAttributes()
{
    int n = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    Debug::Logger::ConsoleOpenGLError("During attempt to get max number of attributes");
    return n;
}

void Controller::Dispose() {
    for (int i = 0; i < _window_count; i++)
        Controller::RemoveWindow(i);
    free(_windows);
}

