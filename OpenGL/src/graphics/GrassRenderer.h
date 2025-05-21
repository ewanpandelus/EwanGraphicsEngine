#pragma once
#include "Shader.h"
#include "Model.h"

class GrassRenderer
{
public:
	GrassRenderer();

	void initialise(glm::vec3 startingPosition, glm::vec2 coverageArea, glm::mat4 projectionMatrix);
	
	void render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec4 clippingPlane);

private:
	std::vector<glm::vec3> m_grassPositions;
	int m_grassCount = 10000;
	Model m_grassModel;
	Shader m_grassShader;
};

