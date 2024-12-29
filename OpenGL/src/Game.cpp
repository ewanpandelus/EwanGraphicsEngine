#include "Game.h"



Game::Game()
{
}

void Game::initialise(int width, int height, GLFWwindow* window)
{ 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    renderer.initialise();
    waterFrameBuffers.initialise();

    treePositions.push_back(glm::vec3(5, 5, 100));
    treePositions.push_back(glm::vec3(100, 5, 5));
    treePositions.push_back(glm::vec3(100, 5, 100));
    treePositions.push_back(glm::vec3(50, 5, 50));
    treePositions.push_back(glm::vec3(30, 5, 15));
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

    // bind to framebuffer and draw scene as we normally would to color texture REFLECTION RENDER

    waterFrameBuffers.bindReflectionFrameBuffer();
    glEnable(GL_DEPTH_TEST);

    // make sure we clear the framebuffer's content
    renderer.prepare(camera.getView());

    // FIRST RENDER - REFLECTION
    renderer.renderReflectionPass();

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


    //if (fmod(floor(totalTime), 2) == 1)
    //{
    //    renderer.renderToScreen(waterFrameBuffers.getRefractionDepthTexture());	// use the color attachment texture as the texture of the quad plane
    //}
    //else
    //{
    //    renderer.renderToScreen(waterFrameBuffers.getRefractionTexture());	// use the color attachment texture as the texture of the quad plane
    //}
    glDisable(GL_CLIP_DISTANCE0);
    /// Last render - what we see 
    renderer.renderOpaqueObjects();
    renderer.renderWater(waterFrameBuffers.getReflectionTexture(), waterFrameBuffers.getRefractionTexture(), waterFrameBuffers.getRefractionDepthTexture());
}

void Game::cleanUp()
{
    //create shutDown for data types 
}
