#include "Game.h"



Game::Game()
{
}

void Game::initialise(int width, int height, GLFWwindow* window)
{ 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    renderer.initialise();
    renderer.setCamera(& camera);
    waterFrameBuffers.initialise();
}

void Game::tick(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    totalTime += deltaTime;

    inputManager.updateInputCommands(window);
    camera.updateCameraPosition(&inputManager, deltaTime);
    render();
    glEnd();
    glfwSwapBuffers(window);

    glfwPollEvents();
    cleanUp();
}

void Game::render()
{
    glm::vec3 cameraPos = camera.getCameraPos();
    float distance = 2 * (cameraPos.y - renderer.getWater()->getPosition().y);
    cameraPos.y -= distance;
    camera.setCameraPos(cameraPos);
    camera.invertPitch();

    // Bind to framebuffer and draw scene 
    waterFrameBuffers.bindReflectionFrameBuffer();
    glEnable(GL_DEPTH_TEST);

    // Clear the framebuffer's content
    renderer.prepare(camera.getView());

    //// FIRST RENDER - REFLECTION
    renderer.renderReflectionPass();

    // Unbind framebuffer and draw scene 
    waterFrameBuffers.unbindCurrentFrameBuffer();
    waterFrameBuffers.bindRefractionFrameBuffer();
    glEnable(GL_DEPTH_TEST);


     //// SECOND RENDER REFRACTION
    cameraPos.y += distance;
    camera.setCameraPos(cameraPos);
    camera.invertPitch();


    renderer.prepare(camera.getView());
    renderer.renderRefractionPass();


    waterFrameBuffers.unbindCurrentFrameBuffer();
    renderer.prepare(camera.getView());

    glDisable(GL_CLIP_DISTANCE0);
    /// Last render - what we see 
    glEnable(GL_DEPTH_TEST);
    renderer.renderOpaqueObjects(glm::vec4(0,0,0,0));
    renderer.renderWater(waterFrameBuffers.getReflectionTexture(), waterFrameBuffers.getRefractionTexture(), waterFrameBuffers.getRefractionDepthTexture());
}

void Game::cleanUp()
{
    //create shutDown for data types 
}

