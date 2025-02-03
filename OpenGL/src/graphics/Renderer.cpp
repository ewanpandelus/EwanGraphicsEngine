#include "Renderer.h"


Renderer::Renderer()
{
}

void Renderer::prepare(glm::mat4 currentView)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    m_currentView = currentView;
}
void Renderer::initialise()
{
    moon = Light(glm::vec3(0, 1200, 4990), glm::vec3(0.79f, 0.791f, 1.0f));
    sun = Light(glm::vec3(0, 400, 1000000), glm::vec3(0.95f, 0.9f, 0.525f));


    terrain = new Terrain(512, 16, glm::vec2(0,0));
    water = new Water(16, 512);
    waterModel = glm::translate(waterModel, glm::vec3(0, 6, 0));

    shader.initialise("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    screenShader.initialise("src/shaders/vsScreen.glsl", "src/shaders/fsScreen.glsl");

    terrainNormalMap = TextureLoader::loadTexture("resources/textures/terrainNormalMap2.png");

    terrainShader.initialise("src/shaders/vsTerrainShader.glsl", "src/shaders/fsTerrainShader.glsl");
    terrainShader.activate();
    terrainShader.setMatrix4("projection", projection);
    terrainShader.setMatrix4("model", *terrain->GetModelMatrix());
    terrainShader.setVector3("lightPos", sun.getLightPosition());
    terrainShader.setVector3("lightColor", moon.getLightColour());
    terrainShader.setInt("normalMap", 0);



    waterShader.initialise("src/shaders/vsWaterShader.glsl", "src/shaders/fsWaterShader.glsl");
    waterShader.activate();
    waterShader.setMatrix4("projection", projection);
    waterShader.setMatrix4("model", waterModel);
    waterShader.setInt("reflectionTexture", 0);
    waterShader.setInt("refractionTexture", 1);
    waterShader.setInt("dudvMap", 2);
    waterShader.setInt("depthTexture", 3);
    waterShader.setInt("normalMap", 4);


    boatShader.initialise("src/shaders/vsBoatShader.glsl", "src/shaders/fsBoatShader.glsl");
    boatShader.activate();
    boatShader.setMatrix4("projection", projection);

    shader.activate();
    shader.setInt("texture1", 0);

    screenShader.activate();
    screenShader.setInt("screenTexture", 0);

    skyboxCube.getShader()->initialise("src/shaders/vsSkybox.glsl", "src/shaders/fsSkybox.glsl");
    skyboxCube.getShader()->setMatrix4("projection", projection);
    skyboxCube.getShader()->setInt("cubeMap", 0);
    skyboxCube.generateBuffers();

    testCube.getShader()->initialise("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    testCube.getShader()->setMatrix4("projection", projection);
    testCube.generateBuffers();


    std::vector<std::string> skyBoxFaces
    {
        "resources/textures/sunsetRight.png",
        "resources/textures/sunsetLeft.png",
        "resources/textures/sunsetTop.png",
        "resources/textures/sunsetBottom.png",
        "resources/textures/sunsetFront.png",
        "resources/textures/sunsetBack.png"
    };

    skyBoxTexture = TextureLoader::loadCubeMapTexture(skyBoxFaces);
    dudvMap = TextureLoader::loadTexture("resources/textures/DuDvMap.png");
    normalMap = TextureLoader::loadTexture("resources/textures/highResNormalMap.png");
   



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    


    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

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

void Renderer::renderOpaqueObjects(glm::vec4 clippingPlane)
{
    terrainShader.activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrainNormalMap);
    terrain->render();

    testCube.getShader()->activate();
    testCube.getShader()->setMatrix4("view", glm::mat3(camera->getView()));
    testCube.getShader()->setMatrix4("projection", projection);
    testCube.getShader()->setMatrix4("model", *testCube.getModelMatrix());
    testCube.bindVertexArray();
    testCube.render();


    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxCube.getShader()->activate();
    skyboxCube.getShader()->setMatrix4("view", glm::mat3(camera->getView()));
    skyboxCube.getShader()->setMatrix4("projection", projection);
    skyboxCube.bindVertexArray();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
    skyboxCube.render();
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Renderer::renderRefractionPass()
{
    terrainShader.activate();
    glm::vec4 clippingPlane = glm::vec4(0.f, -1.f, 0.f, 6.f);
    terrainShader.setVector4("clippingPlane", clippingPlane);
    terrainShader.setMatrix4("view", camera->getView());
    renderOpaqueObjects(clippingPlane);
}

void Renderer::renderReflectionPass()
{
    glEnable(GL_CLIP_DISTANCE0);
    glm::vec4 clippingPlane = glm::vec4(0.f, 1.f, 0.f, -6.f);
    terrainShader.activate();
    terrainShader.setMatrix4("view", camera->getView());
    terrainShader.setVector4("clippingPlane", clippingPlane);
    renderOpaqueObjects(clippingPlane);
}

void Renderer::renderWater(unsigned int reflectionTexture, unsigned int refractionTexture, unsigned int depthTexture)
{
    glDepthMask(false); 
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.f, 1.f);

    waterShader.activate();
    waterShader.setMatrix4("view", camera->getView());
    waterShader.setVector3("lightColour", sun.getLightColour());
    waterShader.setVector3("lightPosition", sun.getLightPosition());
    waterShader.setVector3("viewPos", camera->getCameraPos());

    waterShader.setFloat("time", glfwGetTime()) ;
    waterShader.setVector3("cameraPosition", camera->getCameraPos());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water->render();

    glDepthMask(true); //disable z-testing
    glDisable(GL_BLEND);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void Renderer::renderToScreen(unsigned int textureToBind)
{
    screenShader.activate();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureToBind);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

