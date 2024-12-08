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
		m_cameraSensitivity = 0.2f;
		m_cameraSpeed = 100.f;
		m_yaw = -90.0f;
		m_pitch = 0;
		m_roll = 0;
		m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_direction.y = sin(glm::radians(m_pitch));
		m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	}

	const void updateCameraOrientation(float xMouseOffset, float yMouseOffset);
	const void updateCameraPosition(InputManager* InputManager, float deltaTime);


	const glm::vec3 getCameraPos() const { return m_cameraPos; }
	const glm::vec3 getCameraFront() const { return m_cameraFront; }
	const glm::vec3 getCameraUp() const { return m_cameraUp; }
	const glm::mat4 getView() const { return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp); }

	const void invertPitch() { this->m_pitch = -m_pitch; }

    void setCameraPos(glm::vec3 cameraPos) { m_cameraPos = cameraPos; }
	void setCameraFront(glm::vec3 cameraFront) { m_cameraFront = cameraFront; }
	void setCameraUp(glm::vec3 cameraUp) { m_cameraUp = cameraUp; }


private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



	float m_yaw;
	float m_pitch;
	float m_roll;
	glm::vec3 m_direction;
	float m_cameraSpeed; 
	float m_cameraSensitivity;
	float lastX = 400, lastY = 300;

	bool firstMouse = false;
};