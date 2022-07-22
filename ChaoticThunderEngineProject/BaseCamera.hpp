#ifndef BASE_CAMERA
#define BASE_CAMERA

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "Enums.hpp"

class Window;

class BaseCamera
{
protected:
public:
	//options
	float MoveSpeed;
	float Sensitivity;
	float Fov;
	float Near_plane;
	float Far_plane;

	//Angles
	float Yaw;
	float Pitch;

	//attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 RightAxis;
	glm::vec3 UpAxis;
	glm::vec3 WorldUp;

	virtual void ProcessKeyChanged(int key, int scancode, int action, int mods) = 0;
	virtual void ProcessKeyContinuous(Window* window) = 0;
	virtual void ProcessMousePosition(double xoffset, double yoffset) = 0;
	virtual void ProcessMouseKeyChanged(int button, int action, int mods) = 0;
	virtual void ProcessMouseKeyContinuous(Window* window) = 0;
	virtual void ProcessMouseEnterLeave(bool entered) = 0;
	virtual void ProcessScrollWheel(double xoffset, double yoffset) = 0;

	virtual glm::mat4x4 GetProjectionMatrix(Window* window) = 0;
	virtual glm::mat4x4 GetViewMatrix() = 0;
};
#endif 
