#include "InputManager.h"


void InputManager::updateInputCommands(GLFWwindow* window)
{
    forward.commandActive  = glfwGetKey(window, forward.keyBind);
    backward.commandActive = glfwGetKey(window, backward.keyBind);
    left.commandActive     = glfwGetKey(window, left.keyBind);
    right.commandActive    = glfwGetKey(window, right.keyBind);
}
