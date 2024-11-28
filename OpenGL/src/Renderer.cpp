#include "Renderer.h"

void Renderer::prepareForCurrentRenderFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Renderer::prepareForNextRenderFrame(GLFWwindow* window)
{
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::setupObjectsToRender()
{
    terrain = new Terrain{ glm::vec3(0, 1, 0), 256, 0.5f };
    water = new Water{ 256, 0.5f };
    setupModelMatrices();
    terrain->getShader().setMatrix4("projection", m_projection);
    water->getShader().setMatrix4("projection", m_projection);
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/cube.png");
}

void Renderer::setupModelMatrices()
{
    terrainModel = glm::translate(terrainModel, glm::vec3(0, 0, 10));
    terrainModel = glm::scale(terrainModel, glm::vec3(5, 5, 5));
    waterModel = glm::translate(waterModel, glm::vec3(0, 30, 10));
    waterModel = glm::scale(waterModel, glm::vec3(5, 5, 5));
}

void Renderer::setupRenderMatrices()
{
    // Terrain
    terrain->getShader().activate();
    terrain->getShader().setMatrix4("view", m_view);
    terrain->getShader().setVector3("lightPosition", light.getLightPosition());
    terrain->getShader().setVector4("lightColour", light.getLightColour());
    terrain->getShader().setMatrix4("model", terrainModel);

    // Water 
    water->getShader().activate();
    water->getShader().setMatrix4("view", m_view);
    water->getShader().setVector3("lightPosition", light.getLightPosition());
    water->getShader().setVector4("lightColour", light.getLightColour());
    water->getShader().setMatrix4("model", waterModel);
}

void Renderer::renderOpaqueObjects()
{
    terrain->getShader().activate();
    terrain->render();
    monkeyModel.render();
}

void Renderer::renderTransparentObjects()
{
    water->getShader().activate();
    glDepthMask(false); //disable z-testing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water->render();
    glDepthMask(true); //disable z-testing
    glDisable(GL_BLEND);
}

