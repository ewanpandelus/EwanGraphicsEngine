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
    Light light(glm::vec3(10.0f,10.0f, 10.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f));

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    glm::mat4 view;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);;
    glm::mat4 terrainModel = glm::mat4(1.0f);
    terrainModel = glm::translate(terrainModel, glm::vec3(0, 0, 10));
    terrainModel = glm::scale(terrainModel, glm::vec3(5, 5, 5));

    glm::mat4 waterModel = glm::mat4(1.0f);
    waterModel = glm::translate(waterModel, glm::vec3(0, 30, 10));
    waterModel = glm::scale(waterModel, glm::vec3(5, 5, 5));


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
  
    int width, height;

    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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
  
    Renderer renderer;
    Shader shader("src/shaders/vsShader.glsl", "src/shaders/fsShader.glsl");
    shader.activate();
    shader.setMatrix4("projection", projection);


    Shader waterShader("src/shaders/vsWaterShader.glsl", "src/shaders/fsWaterShader.glsl");
    waterShader.activate();
    waterShader.setMatrix4("projection", projection);

    Terrain terrain(glm::vec3(0, 1, 0), 256, 0.5f);
    Water water(256, 0.5f);

    Model monkeyModel;
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/cube.png");


    WaterFrameBuffers fbos = WaterFrameBuffers();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer.prepare();

        fbos.bindReflectionFrameBuffer();

        fbos.unbindCurrentFrameBuffer();


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputManager.updateInputCommands(window);
        camera.UpdateCameraPosition(&inputManager, deltaTime);


  
      


        view = glm::lookAt(camera.GetCameraPos(), camera.GetCameraPos() + camera.GetCameraFront(), camera.GetCameraUp());
        shader.activate();
        shader.setMatrix4("view", view);
        shader.setVector3("lightPosition", light.getLightPosition());
        shader.setVector4("lightColour", light.getLightColour());
        shader.setMatrix4("model", terrainModel);
        terrain.render();
        monkeyModel.render();


        waterShader.activate();
        waterShader.setMatrix4("view", view);
        waterShader.setVector3("lightPosition", light.getLightPosition());
        waterShader.setVector4("lightColour", light.getLightColour());
        waterShader.setMatrix4("model", waterModel);
        glDepthMask(false); //disable z-testing
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        water.render();
        glDepthMask(true); //disable z-testing
        glDisable(GL_BLEND);
   
        
    
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.UpdateCameraOrientation(xpos, ypos);
}
