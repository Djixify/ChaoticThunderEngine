#include "Camera.hpp"

#include <iostream>

void Camera::ProcessKeyboard(Movement direction, float deltaTime)
{

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
