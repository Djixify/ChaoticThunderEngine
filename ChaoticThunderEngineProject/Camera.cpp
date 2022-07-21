#include "Camera.hpp"

#include <iostream>
#include "Window.hpp"

//vector constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw , float pitch) : 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	MoveSpeed(SPEED), 
	Sensitivity(SENSITIVITY), 
	Fov(FOV),
	Near_plane(NEAR_PLANE),
	Far_plane(FAR_PLANE)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void Camera::ProcessMovement(Window* window, Controls::movement_direction direction, Controls::key_modifier mod)
{
	float velocity = MoveSpeed * window->GetDeltaTimeSec(); 
	if (mod == Controls::key_modifier::SHIFT)
		velocity *= 4;
	if (direction == Controls::movement_direction::FORWARD)
		Position += Front * velocity;
	if (direction == Controls::movement_direction::BACKWARD)
		Position -= Front * velocity;
	if (direction == Controls::movement_direction::LEFT)
		Position -= RightAxis * velocity;
	if (direction == Controls::movement_direction::RIGHT)
		Position += RightAxis * velocity;
}

void Camera::ProcessMouse(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += -yoffset;

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
	Fov -= (float)yoffset;
	if (Fov < 45.0f)
		Fov = 45.0f;
	if (Fov > 135.0f)
		Fov = 135.0f;
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

glm::mat4x4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, WorldUp);
}
