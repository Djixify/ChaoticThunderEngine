#include "Camera.hpp"

#include <iostream>
#include "Enums.hpp"

//vector constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw , float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void Camera::ProcessMovement(Controls::movement_direction direction)
{
	/*
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		Position += Front * MoveSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		Position -= Front * MoveSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		Position -= RightAxis * MoveSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		Position += RightAxis * MoveSpeed;
	*/
}

void Camera::ProcessMouse(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// constrained so that excessive pitch won't flip screen
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
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
