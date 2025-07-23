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
    GLuint m_grassSSBO;
    const int m_grassCount = 200000;
    glm::vec3 m_startPosition;
    glm::vec2 m_coverageArea;

    Shader m_grassShader;
    Shader m_computeShader;
    Model m_grassModel;

    std::vector<glm::vec3> m_grassPositions;
};