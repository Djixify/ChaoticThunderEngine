#ifndef ENGINE_CAMERA
#define ENGINE_CAMERA
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "Enums.hpp"

class Window;

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.001f;
const float SENSITIVITY = 0.01f;
const float FOV = 90.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

class Camera
{
public:
	//function declarations
	void ProcessMovement(Window* window, Controls::movement_direction direction, Controls::key_modifier mod);
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
	float Fov;
	float Near_plane;
	float Far_plane;

	//vector constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	glm::mat4x4 GetViewMatrix();
};
#endif 
