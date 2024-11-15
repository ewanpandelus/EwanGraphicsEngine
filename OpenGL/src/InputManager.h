#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


struct InputCommand
{
	bool commandActive = false;
	int keyBind = 0;
public:
	InputCommand(int keyBind_)
	{
		keyBind = keyBind_;
	}
};


class InputManager 
{
public:
	InputCommand forward{ GLFW_KEY_W };
	InputCommand backward{ GLFW_KEY_S };
	InputCommand left{ GLFW_KEY_A };
	InputCommand right{ GLFW_KEY_D };

	void updateInputCommands(GLFWwindow* window);
};



