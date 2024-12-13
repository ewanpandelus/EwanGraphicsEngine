#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Light.h"
#include "Shader.h"
#include "../Terrain.h"
#include "../water/Water.h"
#include "Model.h"
#include "TextureLoader.h"
#include "Model.h"

class Renderer
{
public:
	Renderer();
	void prepare(glm::mat4 currentView);

	void initialise();

	void renderOpaqueObjects();

	void renderTransparentObjects();

	Water getWater() {return *water;}

	void renderToScreen(unsigned int textureToBind);

private:
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
	glm::mat4 m_currentView;

	// Lighting 
	Light light;

	// Shaders 
	Shader terrainShader;
	Shader treeShader;
	Shader screenShader;
	Shader shader;
	Shader waterShader;

	// Textures
	unsigned int cubeTexture;
	unsigned int floorTexture;

	// Environment
	Water* water{};
	Terrain* terrain{};



	Model monkeyModel;
	Model tree;

	// Environment Model Matrices
	glm::mat4 terrainModel = glm::mat4(1.0f);
	glm::mat4 waterModel = glm::mat4(1.0f);
	glm::mat4 treeModel = glm::mat4(1.0f);


	// TEMP

	float quadVertices[24];// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	unsigned int quadVAO, quadVBO;
	//
};

