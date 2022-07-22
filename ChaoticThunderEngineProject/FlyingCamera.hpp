#ifndef FLYING_CAMERA
#define FLYING_CAMERA
#include "BaseCamera.hpp"
#include "Enums.hpp"

class Window;

static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 0.001f;
static const float SENSITIVITY = 0.01f;
static const float FOV = 90.0f;
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.0f;

class FlyingCamera : public BaseCamera
{
private:
	bool _constrain_pitch;
public:
	//vector constructor
	FlyingCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	void ProcessKeyChanged(int key, int scancode, int action, int mods);
	void ProcessKeyContinuous(Window* window);
	void ProcessMousePosition(double xoffset, double yoffset);
	void ProcessMouseKeyChanged(int button, int action, int mods);
	void ProcessMouseKeyContinuous(Window* window);
	void ProcessMouseEnterLeave(bool entered);
	void ProcessScrollWheel(double xoffset, double yoffset);

	glm::mat4x4 GetProjectionMatrix(Window* window);
	glm::mat4x4 GetViewMatrix();

	void UpdateCameraVectors();
};
#endif