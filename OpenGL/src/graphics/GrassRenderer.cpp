#include "GrassRenderer.h"

GrassRenderer::GrassRenderer()
{

}

void GrassRenderer::initialise(glm::vec3 startingPosition, glm::vec2 coverageArea, glm::mat4 projectionMatrix)
{
	m_grassModel.prepareModel("resources/objects/grass.obj", "resources/textures/DuDvMap.png");
	m_grassShader.initialise("src/shaders/vsGrassShader.glsl", "src/shaders/fsGrassShader.glsl");
	m_grassShader.activate();
	m_grassShader.setMatrix4("projection", projectionMatrix);

	m_grassPositions.clear();
	m_grassPositions.reserve(m_grassCount);
	for (int grassBlade = 0; grassBlade < m_grassCount; grassBlade++) 
	{
		float randomX = static_cast<float>(rand()) / RAND_MAX * coverageArea.x;
		float randomZ = static_cast<float>(rand()) / RAND_MAX * coverageArea.y;

		glm::vec3 bladePosition = startingPosition + glm::vec3(randomX, 0.0f, randomZ);
		m_grassPositions.push_back(bladePosition);
	}
}

void GrassRenderer::render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec4 clippingPlane)
{
	m_grassShader.activate();
	m_grassShader.setMatrix4("view", viewMatrix);
	m_grassShader.setMatrix4("projection", projectionMatrix);
	m_grassShader.setVector4("clippingPlane", clippingPlane);


	for (const glm::vec3& position : m_grassPositions) 
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, position);
		m_grassShader.setMatrix4("model", model);
		m_grassModel.render();
	}
}
