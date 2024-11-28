#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "InputManager.h"
#include "Renderer.h"
#include "Model.h"
#include "Planet.h"
#include "water/Water.h"
#include "water/WaterFrameBuffers.h"




Camera camera;



void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() 
{
 
    GLFWwindow* window;
    InputManager inputManager;
 


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
  
    Renderer renderer{};

    int width = 800;
    int height = 600;
    float deltaTime = 0.0f;	
    float lastFrame = 0.0f; 
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;
   
    glfwSetCursorPosCallback(window, mouse_callback);
 //   WaterFrameBuffers fbos = WaterFrameBuffers();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    renderer.setupObjectsToRender();
    glEnable(GL_DEPTH_TEST);
    renderer.prepareForCurrentRenderFrame();
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer.prepareForCurrentRenderFrame();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        inputManager.updateInputCommands(window);
        camera.UpdateCameraPosition(&inputManager, deltaTime);
        renderer.setCurrentView(glm::lookAt(camera.GetCameraPos(), camera.GetCameraPos() + camera.GetCameraFront(), camera.GetCameraUp()));
        renderer.setupRenderMatrices();
        renderer.renderOpaqueObjects();
        renderer.renderTransparentObjects();
        renderer.prepareForNextRenderFrame(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.UpdateCameraOrientation(xpos, ypos);
}
