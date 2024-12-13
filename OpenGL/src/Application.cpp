#include <iostream>
#include <GL/glew.h>
#include "graphics/stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Game.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Game game;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() 
{
 
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
  
    int width = 800, height = 600;

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
  

    game.initialise(width, height, window);


    while (!glfwWindowShouldClose(window))
    {
        game.tick(window);
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    game.getCamera()->updateCameraOrientation(xpos, ypos);
}
