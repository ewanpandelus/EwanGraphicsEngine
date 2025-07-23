#include "GrassRenderer.h"

GrassRenderer::GrassRenderer()
    : m_grassSSBO(0)
{}

void GrassRenderer::initialise(glm::vec3 startingPosition, glm::vec2 coverageArea, glm::mat4 projectionMatrix)
{
    m_startPosition = startingPosition;
    m_coverageArea = coverageArea;

    m_grassModel.prepareModel("resources/objects/grass.obj", "resources/textures/DuDvMap.png");

    m_grassShader.initialise("src/shaders/vsGrassShader.glsl", "src/shaders/fsGrassShader.glsl");
    m_grassShader.activate();
    m_grassShader.setMatrix4("projection", projectionMatrix);

    m_computeShader.initialiseCompute("src/shaders/computeGrassShader.comp");
    m_computeShader.activate();

    // Create and bind SSBO
    glGenBuffers(1, &m_grassSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_grassSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_grassCount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_grassSSBO);

    // Dispatch compute shader
    m_computeShader.setVector3("startPosition", m_startPosition);
    m_computeShader.setVector2("coverage", m_coverageArea);
    m_computeShader.setUInt("seed", static_cast<unsigned int>(time(nullptr)));
    m_computeShader.setUInt("maxGrassCount", m_grassCount);
    m_computeShader.setFloat("areaSize", 1.0f); 

    int groups = (m_grassCount + 255) / 256;
    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


void GrassRenderer::render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec4 clippingPlane)
{
	m_grassShader.activate();
	m_grassShader.setMatrix4("view", viewMatrix);
    m_grassShader.setMatrix4("model", glm::mat4(1.0f)); 
	

    glBindVertexArray(m_grassModel.getVAO());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_grassSSBO);
    glDrawElementsInstanced(GL_TRIANGLES, m_grassModel.getIndexCount(), GL_UNSIGNED_INT, 0, m_grassCount);
}
