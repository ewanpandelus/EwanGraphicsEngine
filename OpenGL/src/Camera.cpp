#include "Camera.h"


const void Camera::updateCameraOrientation(float xPosIn, float yPosIn)
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

    xoffset *= m_cameraSensitivity;
    yoffset *= m_cameraSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(front);
}

const void Camera::updateCameraPosition(InputManager* inputManager, float deltaTime)
{

    if (inputManager->forward.commandActive)
        m_cameraPos = ((m_cameraPos) += (m_cameraSpeed * deltaTime * m_cameraFront));

    if (inputManager->backward.commandActive)
        m_cameraPos = ((m_cameraPos) -= (m_cameraSpeed * deltaTime * m_cameraFront));

    if (inputManager->left.commandActive)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed * deltaTime;

    if (inputManager->right.commandActive)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed * deltaTime;
}
