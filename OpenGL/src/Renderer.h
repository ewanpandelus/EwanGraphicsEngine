#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Terrain.h"
#include "water/Water.h"
#include "Model.h"
#include "Light.h"

class Renderer
{
public:

	void prepareForCurrentRenderFrame();
    void prepareForNextRenderFrame(GLFWwindow* window);
    void const setCurrentView(glm::mat4 view) { m_view = view;}
	void setupObjectsToRender();
	void setupModelMatrices();
    void setupRenderMatrices();
    void renderOpaqueObjects();
    void renderTransparentObjects();

private:

    glm::mat4 m_view;
    glm::mat4 m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);;
    glm::mat4 terrainModel = glm::mat4(1.0f);
    glm::mat4 waterModel = glm::mat4(1.0f);


    /// Objects to render 
    Terrain* terrain = nullptr; //{ glm::vec3(0, 1, 0), 256, 0.5f };
    Water* water = nullptr; //{ 256, 0.5f };
    Model monkeyModel;

    /// Extra 
    Light light{ glm::vec3(10.0f, 10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };

};

