#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "InputManager.h"

class Camera 
{
public:
	Camera() 
	{
		cameraSensitivity = 0.2f;
		cameraSpeed = 100.f;
		yaw= -90.0f;
		pitch = 0;
		roll = 0;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	}

	void UpdateCameraOrientation(float xMouseOffset, float yMouseOffset);
	
	void UpdateCameraPosition(InputManager* InputManager, float deltaTime);


	glm::vec3 GetCameraPos() const { return m_cameraPos; }
	glm::vec3 GetCameraFront() const { return m_cameraFront; }
	glm::vec3 GetCameraUp() const { return m_cameraUp; }

	glm::vec3 SetCameraPos(glm::vec3 cameraPos) { m_cameraPos = cameraPos; }

	glm::vec3 SetCameraFront(glm::vec3 cameraFront)  { m_cameraFront = cameraFront; }
	glm::vec3 SetCameraUp(glm::vec3 cameraUp) { m_cameraUp = cameraUp; }


private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


	bool firstMouse = false;

	float yaw;
	float pitch;
	float roll;
	glm::vec3 direction;

	float cameraSpeed; 
	float cameraSensitivity;


	float lastX = 400, lastY = 300;
};