#include <iostream>
#include "OrbitingCamera.hpp"
#include "Window.hpp"

OrbitingCamera::OrbitingCamera(glm::vec3 position, glm::vec3 up, glm::vec3 pivot, float yaw, float pitch)
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MoveSpeed = speed;
	Sensitivity = orbitspeed;
	Fov = fov;
	Near_plane = nearplane;
	Far_plane = farplane;
	_constrain_pitch = true;
	_attach_mouse = false;
	Pivot = pivot;
	Radius = 1.0f;


	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

void OrbitingCamera::ProcessKeyContinuous(Window* window) {
	/*GLFWwindow* _glfwwindow = window->GetGLContext();
	float radius = 1.0f;
	
	bool orbit = glfwGetKey(_glfwwindow, (int)Controls::key::O) == GLFW_PRESS;
	float velocity = MoveSpeed * window->GetDeltaTimeSec();
	if (orbit) {
		Position.x = sinf(glfwGetTime()) * radius;
		Position.y = 0.0f;
		Position.z = cosf(glfwGetTime()) * radius;
	}

	if (glfwGetKey(_glfwwindow, (int)Controls::key::W) == GLFW_PRESS)
		Position += Front * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::S) == GLFW_PRESS)
		Position -= Front * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::A) == GLFW_PRESS)
		Position -= RightAxis * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::D) == GLFW_PRESS)
		Position += RightAxis * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::SPACE) == GLFW_PRESS)
		Position += UpAxis * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::LEFT_CONTROL) == GLFW_PRESS)
		Position -= UpAxis * velocity;

	Position = glm::normalize(Position) * radius;
	UpdateCameraVectors();*/
}

void OrbitingCamera::ProcessScrollWheel(double xoffset, double yoffset)
{
	Fov -= (float)yoffset;
	if (Fov < 45.0f)
		Fov = 45.0f;
	if (Fov > 135.0f)
		Fov = 135.0f;
}

glm::mat4x4 OrbitingCamera::GetProjectionMatrix(Window* window) {
	return glm::perspective(glm::radians(Fov), window->GetAspectRatio(), Near_plane, Far_plane);
}

glm::mat4x4 OrbitingCamera::GetViewMatrix() {
	return glm::lookAt(Position, Pivot , WorldUp);
}

void OrbitingCamera::ProcessKeyChanged(int key, int scancode, int action, int mods) {
	if (key == (int)Controls::key::Q && action == (int)Controls::key_interaction::RELEASE)
		_attach_mouse = true;
	else if (key == (int)Controls::key::E && action == (int)Controls::key_interaction::RELEASE)
		_attach_mouse = false;
}

void OrbitingCamera::ProcessMousePosition(double xoffset, double yoffset) {
	if (!_attach_mouse)
		return;

	xoffset *= 0.1 * Sensitivity;
	yoffset *= 0.1 * Sensitivity;

	Position += RightAxis * (float)xoffset;
	Position += UpAxis * (float)yoffset;
	Position = glm::normalize(Position) * Radius;
	// constrained so that excessive pitch won't flip screen
	if (_constrain_pitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	UpdateCameraVectors();
}
void OrbitingCamera::ProcessMouseKeyChanged(int button, int action, int mods) {

}
void OrbitingCamera::ProcessMouseKeyContinuous(Window* window) {

}
void OrbitingCamera::ProcessMouseEnterLeave(bool entered) {

}

void OrbitingCamera::UpdateCameraVectors()
{
	//updates Direction, Right and Up Axis vectors
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	
	Front = glm::normalize(front);
	RightAxis = glm::normalize(glm::cross(Front, WorldUp));
	UpAxis = glm::normalize(glm::cross(RightAxis, Front));
}