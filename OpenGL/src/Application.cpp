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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, 10));


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
  
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
  

    Planet planet(64, 200);
    planet.constructMeshes();

    Renderer renderer;
    Shader shader("src/shaders/vsShader.glsl", "src/shaders/fsShader.glsl");
    shader.activate();
    shader.setMatrix4("projection", projection);


    //Model cubeModel;
    //cubeModel.prepareModel("resources/Crate.obj", "resources/textures/cube.png");


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer.prepare();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputManager.updateInputCommands(window);
        camera.UpdateCameraPosition(&inputManager, deltaTime);


        view = glm::lookAt(camera.GetCameraPos(), camera.GetCameraPos() + camera.GetCameraFront(), camera.GetCameraUp());
        shader.setMatrix4("view", view);
        shader.setVector3("lightPosition", light.getLightPosition());
        shader.setVector4("lightColour", light.getLightColour());
     
       
      
       
        shader.setMatrix4("model", model);
        planet.render();


  //      float timeValue = glfwGetTime();
  

   

        glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    planet.cleanUp();
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.UpdateCameraOrientation(xpos, ypos);
}
