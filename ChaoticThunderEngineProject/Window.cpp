#include <map>
#include <utility>
#include "Window.hpp"
#include "Debug.hpp"
#include "Controller.hpp"
#include "Shader.hpp"
#include "FlyingCamera.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void _processKeyChanged(GLFWwindow* glfwwindow, int key, int scancode, int action, int mods) {
	for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
		Window* window = Controller::Instance()->GetWindow(i);
		if (window->GetGLContext() == glfwwindow)
			Controller::Instance()->GetContextWindow()->ProcessKeyChanged(glfwwindow, key, scancode, action, mods);
	}
}

void _processMousePosChanged(GLFWwindow* glfwwindow, double xpos, double ypos) {
	for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
		Window* window = Controller::Instance()->GetWindow(i);
		if (window->GetGLContext() == glfwwindow)
			Controller::Instance()->GetContextWindow()->ProcessMousePosition(glfwwindow, xpos, ypos);
	}
}

void _processMouseEnterLeaveChanged(GLFWwindow* glfwwindow, int entered) {
	for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
		Window* window = Controller::Instance()->GetWindow(i);
		if (window->GetGLContext() == glfwwindow)
			Controller::Instance()->GetContextWindow()->ProcessMouseEnterLeave(glfwwindow, entered == true);
	}
}

void _processMouseKeyChanged(GLFWwindow* glfwwindow, int button, int action, int mods) {
	for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
		Window* window = Controller::Instance()->GetWindow(i);
		if (window->GetGLContext() == glfwwindow)
			Controller::Instance()->GetContextWindow()->ProcessMouseKeyChanged(glfwwindow, button, action, mods);
	}
}

void _processMouseWheelChanged(GLFWwindow* glfwwindow, double xoffset, double yoffset) {
	for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
		Window* window = Controller::Instance()->GetWindow(i);
		if (window->GetGLContext() == glfwwindow)
			Controller::Instance()->GetContextWindow()->ProcessScrollWheel(glfwwindow, xoffset, yoffset);
	}
}

Window::Window(std::string title, int width, int height, Window* other) :
	_activeshader(0),
	_environment_fill_mesh(true),
	_start_time(std::clock()),
	_delta_time(.0f),
	_current_time(.0f),
	_prev_pos(glm::vec2(.0f)),
	_invalid_prev_pos(true)
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
	BaseCamera* camera = new FlyingCamera();
	_cameras.push_back(std::shared_ptr<BaseCamera>(camera));
	_activecamera = 0;

	//Add bindings to events

	//UpdateShaderFiles();
	//window->AddShader("Custom", 2, load_shader{ shader_type::VERTEX, vertex_shaders[selected_vertex_shader] }, load_shader{ shader_type::FRAGMENT, fragment_shaders[selected_fragment_shader] });
	//programs = _glfwwindow->GetShaderLabels();
	//typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
	glfwSetKeyCallback(_glfwwindow, _processKeyChanged);
	glfwSetCursorPosCallback(_glfwwindow, _processMousePosChanged);
	glfwSetCursorEnterCallback(_glfwwindow, _processMouseEnterLeaveChanged);
	glfwSetMouseButtonCallback(_glfwwindow, _processMouseKeyChanged);
	glfwSetScrollCallback(_glfwwindow, _processMouseWheelChanged);

	_start_time = std::clock();
}

Window::~Window() {
	_shaders.clear();
	_cameras.clear();
}

void Window::ProcessKeyChanged(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Debug::Logger::Console(Debug::Level::INFO, "Keyboard key pressed: key %d, scancode %d, action %d, mods %d", key, scancode, action, mods);

	if (key == (int)Controls::key::Q)
		SetCursorInputType(Controls::cursor_input_type::CENTERED);
	else if (key == (int)Controls::key::E)
		SetCursorInputType(Controls::cursor_input_type::NORMAL);

	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessKeyChanged(key, scancode, action, mods);
}

void Window::ProcessKeyContinuous()
{
	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessKeyContinuous(this);
}

void Window::ProcessMousePosition(GLFWwindow* window, double xpos, double ypos)
{
	//Debug::Logger::Console(Debug::Level::INFO, "New mouse pos: %f %f", xpos, ypos);

	if (_invalid_prev_pos) {
		_prev_pos.x = xpos;
		_prev_pos.y = ypos;
		_invalid_prev_pos = false;
		return;
	}

	//Do magic with front facing vector
	glm::vec2 current_pos = glm::vec2(xpos, ypos);
	glm::vec2 delta_pos = current_pos - _prev_pos;
	//Debug::Logger::Console(Debug::Level::INFO, "Delta mouse pos: %f %f", delta_pos.x, delta_pos.y);

	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessMousePosition(delta_pos.x, delta_pos.y);

	_prev_pos = current_pos;
}

void Window::ProcessMouseKeyChanged(GLFWwindow* window, int button, int action, int mods)
{
	Debug::Logger::Console(Debug::Level::INFO, "Mouse key pressed: button %d, action %d, mods %d", button, action, mods);

	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessMouseKeyChanged(button, action, mods);
}

void Window::ProcessMouseKeyContinuous()
{
	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessMouseKeyContinuous(this);
}

void Window::ProcessMouseEnterLeave(GLFWwindow* window, bool entered)
{
	Debug::Logger::Console(Debug::Level::INFO, entered ? "Entered the window" : "Left the window");
	if (!entered)
		_invalid_prev_pos = true;
}

void Window::ProcessScrollWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	Debug::Logger::Console(Debug::Level::INFO, "Scroll wheel changed: %f %f", xoffset, yoffset);

	BaseCamera* camera = Controller::Instance()->GetContextWindow()->GetActiveCamera();
	camera->ProcessScrollWheel(xoffset, yoffset);
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

glm::vec2 Window::GetMousePos() {
	return glm::vec2(_prev_pos.x, -_prev_pos.y);
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

float Window::UpdateTime() {
	clock_t now = std::clock();
	_delta_time = ((float)(now - _current_time)) / 1000.0f;
	_current_time = ((float)(now - _start_time)) / 1000.f;
	return _current_time;
}

float Window::GetCurrentTimeSec() {
	return _current_time;
}

float Window::GetDeltaTimeSec() {
	return _delta_time;
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
	glfwSetInputMode(_glfwwindow, GLFW_CURSOR, (int)input_type);
	_invalid_prev_pos = true;
}

void Window::AddShader(std::string shader_name, std::vector<load_shader> shaders)
{
	_shaders.insert_or_assign(shader_name, std::make_shared<Shader>(this, shaders));
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

/*
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
*/

BaseCamera* Window::GetActiveCamera() {
	return _cameras[_activecamera].get();
}