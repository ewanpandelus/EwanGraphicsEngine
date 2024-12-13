#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	void prepare();

	void initialiseShaders();

	void renderOpaqueObjects();

	void renderTransparentObjects();
};

