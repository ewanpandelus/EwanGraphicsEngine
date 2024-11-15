#include "Camera.h"


void Camera::UpdateCameraOrientation(float xPosIn, float yPosIn)
{
    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    // refactor 
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= cameraSensitivity;
    yoffset *= cameraSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_cameraFront = glm::normalize(front);
}

void Camera::UpdateCameraPosition(InputManager* inputManager, float deltaTime)
{

    if (inputManager->forward.commandActive)
        m_cameraPos = ((m_cameraPos) += (cameraSpeed * deltaTime * m_cameraFront));

    if (inputManager->backward.commandActive)
        m_cameraPos = ((m_cameraPos) -= (cameraSpeed * deltaTime * m_cameraFront));

    if (inputManager->left.commandActive)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed * deltaTime;

    if (inputManager->right.commandActive)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed * deltaTime;
}
