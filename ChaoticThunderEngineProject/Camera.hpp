#ifndef ENGINE_CAMERA
#define ENGINE_CAMERA
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "Window.hpp"

enum Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
	//function declarations
	void ProcessKeyboard(Movement direction, float deltaTime);
	void ProcessMouse(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessMouseScroll(float yoffset);
	void updateCameraVectors();

	//Angles
	float Yaw;
	float Pitch;

	//attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 RightAxis;
	glm::vec3 UpAxis;
	glm::vec3 WorldUp;

	//options
	float MoveSpeed;
	float Sensitivity;
	float Zoom;

	//vector constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//scalar constructor
	Camera(float x, float y, float z, float up_x, float up_y, float up_z, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(x, y, z);
		WorldUp = glm::vec3(up_x, up_y, up_z);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

	}
	


};
#endif 
