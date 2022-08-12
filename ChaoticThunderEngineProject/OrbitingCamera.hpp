#ifndef ORBITCAMERA
#define ORBITCAMERA
#include "BaseCamera.hpp"
#include "Enums.hpp"
class Window;

static const float pitch = 0.0f;
static const float yaw = -90.0f;
static const float fov = 90.0f;
static const float speed = 0.001f;
static const float nearplane = 0.1f;
static const float farplane = 100.0f;
static const float orbitspeed = 0.02f;

class OrbitingCamera : public BaseCamera
{
private:
	bool _constrain_pitch;
	bool _attach_mouse;
public:
	OrbitingCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = yaw, float pitch = pitch);

	glm::vec3 Pivot;
	float Radius;

	void OrbitTrajectory(Window* window, float _orbit_speed);
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