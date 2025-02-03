#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Light.h"
#include "Shader.h"
#include "../terrain/Terrain.h"
#include "Model.h"
#include "TextureLoader.h"
#include "../water/Water.h"
#include "../Camera.h"
#include "Primitives.h"

class Renderer
{
public:
	Renderer();
	void prepare(glm::mat4 currentView);

	void initialise();

	void renderOpaqueObjects(glm::vec4 clippingPlane);

	void renderRefractionPass();

	void renderReflectionPass();

	void renderWater(unsigned int reflectionTexture, unsigned int refractionTexture, unsigned int depthTexture);

	void renderToScreen(unsigned int textureToBind);

	Water* getWater() { return water; }

	void setCamera(Camera* camera) { this->camera = camera;}

private:
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10000.0f);
	glm::mat4 m_currentView;

	Camera* camera;

	// Lighting 
	Light moon;
	Light sun;


	// Shaders 

	Shader terrainShader;
	Shader boatShader;
	Shader screenShader;
	Shader shader;
	Shader waterShader;

	// Textures
	unsigned int skyBoxTexture;
	unsigned int dudvMap;
	unsigned int normalMap;
	unsigned int terrainNormalMap;

	// Environment
	Water* water{};
	Terrain* terrain{};
	
	// Primitives
	Cube skyboxCube{};
	Cube testCube{ glm::vec3(0,400, 30000), 100};


	Model monkeyModel;
	Model boatModel;
	Model tree;

	// Environment Model Matrices

	glm::mat4 waterModel = glm::mat4(1.0f);
	glm::mat4 treeModel = glm::mat4(1.0f);


	unsigned int planeVAO, planeVBO;
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	//
};

