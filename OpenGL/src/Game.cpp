#include "Game.h"



Game::Game()
{
}

void Game::initialise(int width, int height, GLFWwindow* window)
{ 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    light = Light(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    shader.initialise("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    screenShader.initialise("src/shaders/vsScreen.glsl", "src/shaders/fsScreen.glsl");

    terrainShader.initialise("src/shaders/vsTerrainShader.glsl", "src/shaders/fsTerrainShader.glsl");
    terrainShader.activate();
    terrainShader.setMatrix4("projection", projection);

    waterShader.initialise("src/shaders/vsWaterShader.glsl", "src/shaders/fsWaterShader.glsl");
    waterShader.activate();
    waterShader.setMatrix4("projection", projection);

    treeShader.initialise("src/shaders/vsTreeShader.glsl", "src/shaders/fsTreeShader.glsl");
    treeShader.activate();
    treeShader.setMatrix4("projection", projection);


    terrain = Terrain(glm::vec3(0, 1, 0), 256, 0.5f);
    water  = Water(256, 0.5f);
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/Crate.png");
    Model tree;
    tree.prepareModel("resources/objects/tree.obj", "resources/textures/Crate.png");

    waterModel = glm::translate(waterModel, water.GetPosition());

    shader.activate();
    shader.setInt("texture1", 0);

    screenShader.activate();
    screenShader.setInt("screenTexture", 0);


    unsigned int cubeTexture = TextureLoader::loadTexture("resources/textures/container.png");
    unsigned int floorTexture = TextureLoader::loadTexture("resources/textures/wall.png");

    waterFrameBuffers.initialise();

    treePositions.push_back(glm::vec3(5, 5, 100));
    treePositions.push_back(glm::vec3(100, 5, 5));
    treePositions.push_back(glm::vec3(100, 5, 100));
    treePositions.push_back(glm::vec3(50, 5, 50));
    treePositions.push_back(glm::vec3(30, 5, 15));



    float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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
    float distance = 2 * (cameraPos.y - water.GetPosition().y);
    cameraPos.y -= distance;
    camera.setCameraPos(cameraPos);
    camera.invertPitch();

    // bind to framebuffer and draw scene as we normally would to color texture REFLECTION RENDER

    waterFrameBuffers.bindReflectionFrameBuffer();
    glEnable(GL_DEPTH_TEST);

    // make sure we clear the framebuffer's content
    renderer.prepare();
    // FIRST RENDER - REFLECTION
    glm::mat4 view;
    view = camera.getView();
    shader.activate();
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);

    terrainShader.activate();
    terrainShader.setMatrix4("model", glm::mat4(1.0f));
    terrainShader.setMatrix4("view", view);
    terrainShader.setVector3("lightPosition", light.getLightPosition());
    terrainShader.setVector4("lightColour", light.getLightColour());
    terrainShader.setMatrix4("model", terrainModel);
    terrain.render();

    treeShader.activate();
    treeShader.setMatrix4("model", glm::mat4(1.0f));
    treeShader.setMatrix4("view", view);
    treeShader.setVector3("lightPosition", light.getLightPosition());
    treeShader.setVector4("lightColour", light.getLightColour());
    treeShader.setMatrix4("model", treeModel);
    tree.render();

    waterFrameBuffers.unbindCurrentFrameBuffer();
    waterFrameBuffers.bindRefractionFrameBuffer();
    glEnable(GL_DEPTH_TEST);
    renderer.prepare();

     //// SECOND RENDER REFRACTION
    cameraPos.y += distance;
    camera.setCameraPos(cameraPos);
    camera.invertPitch();
    view = camera.getView();

    shader.activate();
    shader.setMatrix4("view", view);
    shader.setVector3("lightPosition", light.getLightPosition());
    shader.setVector4("lightColour", light.getLightColour());
    shader.setMatrix4("model", terrainModel);

    terrainShader.activate();
    terrainShader.setMatrix4("model", glm::mat4(1.0f));
    terrainShader.setMatrix4("view", view);
    terrainShader.setVector3("lightPosition", light.getLightPosition());
    terrainShader.setVector4("lightColour", light.getLightColour());
    terrainShader.setMatrix4("model", terrainModel);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);	// use the color attachment texture as the texture of the quad plane
    terrain.render();


    waterFrameBuffers.unbindCurrentFrameBuffer();
    renderer.prepare();

     // Screen space quad with frame buffer texture 
     screenShader.activate();
     glBindVertexArray(quadVAO);
     if (fmod(floor(totalTime), 2) == 1)
     {
         glBindTexture(GL_TEXTURE_2D, waterFrameBuffers.getReflectionTexture());	// use the color attachment texture as the texture of the quad plane
     }
     else
     {
          glBindTexture(GL_TEXTURE_2D, waterFrameBuffers.getRefractionTexture());	// use the color attachment texture as the texture of the quad plane
     }
    glDrawArrays(GL_TRIANGLES, 0, 6);

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


}

void Game::cleanUp()
{
    //create shutDown for data types 
}
