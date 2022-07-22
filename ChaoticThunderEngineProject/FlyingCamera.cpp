
#include <iostream>
#include "Window.hpp"
#include "FlyingCamera.hpp"

//vector constructor
FlyingCamera::FlyingCamera(glm::vec3 position, glm::vec3 up, float yaw , float pitch)
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MoveSpeed = SPEED;
	Sensitivity = SENSITIVITY;
	Fov = FOV;
	Near_plane = NEAR_PLANE;
	Far_plane = FAR_PLANE;
	_constrain_pitch = true;

	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

void FlyingCamera::ProcessKeyChanged(int key, int scancode, int action, int mods) {}

void FlyingCamera::ProcessKeyContinuous(Window* window)
{
	GLFWwindow* _glfwwindow = window->GetGLContext();
	
	bool shift_held = glfwGetKey(_glfwwindow, (int)Controls::key::LEFT_SHIFT) == GLFW_PRESS;
	float velocity = MoveSpeed * window->GetDeltaTimeSec(); 
	if (shift_held)
		velocity *= 4;

	if (glfwGetKey(_glfwwindow, (int)Controls::key::W) == GLFW_PRESS)
		Position += Front * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::S) == GLFW_PRESS)
		Position -= Front * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::A) == GLFW_PRESS)
		Position -= RightAxis * velocity;
	if (glfwGetKey(_glfwwindow, (int)Controls::key::D) == GLFW_PRESS)
		Position += RightAxis * velocity;
}

void FlyingCamera::ProcessMousePosition(double xoffset, double yoffset)
{
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += -yoffset;

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

void FlyingCamera::ProcessMouseKeyChanged(int button, int action, int mods) {}

void FlyingCamera::ProcessMouseKeyContinuous(Window* window) {}

void FlyingCamera::ProcessMouseEnterLeave(bool entered) {}

void FlyingCamera::ProcessScrollWheel(double xoffset, double yoffset)
{
	Fov -= (float)yoffset;
	if (Fov < 45.0f)
		Fov = 45.0f;
	if (Fov > 135.0f)
		Fov = 135.0f;
}

glm::mat4x4 FlyingCamera::GetProjectionMatrix(Window* window) {
	return glm::perspective(Fov, window->GetAspectRatio(), Near_plane, Far_plane);
}

glm::mat4x4 FlyingCamera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, WorldUp);
}

void FlyingCamera::UpdateCameraVectors()
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

