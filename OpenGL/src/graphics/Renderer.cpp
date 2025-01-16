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
    light = Light(glm::vec3(0.0f, 100.0f, 50.0f), glm::vec4(0.7f, 0.5f, 0.1f, 1.0f));
    terrainModel = glm::translate(waterModel, glm::vec3(0, -430, 0));
    waterModel = glm::translate(waterModel, glm::vec3(25, 6, 25));

    shader.initialise("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    screenShader.initialise("src/shaders/vsScreen.glsl", "src/shaders/fsScreen.glsl");

    terrainShader.initialise("src/shaders/vsTerrainShader.glsl", "src/shaders/fsTerrainShader.glsl");
    terrainShader.activate();
    terrainShader.setMatrix4("projection", projection);
    terrainShader.setMatrix4("model", terrainModel);

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


    terrain = new Terrain(glm::vec3(0, 1, 0), 256, 4);
    water = new Water(256, 4);
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/Crate.png");
    Model tree;
    tree.prepareModel("resources/objects/tree.obj", "resources/textures/Crate.png");
    boatModel.prepareModel("resources/objects/boat.obj", "resources/textures/Crate.png");

    shader.activate();
    shader.setInt("texture1", 0);

    screenShader.activate();
    screenShader.setInt("screenTexture", 0);

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
    
    cube.getShader()->initialise("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    cube.generateBuffers();

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
    terrain->render();
    cube.getShader()->activate();
    cube.getShader()->setModelViewProjection(*cube.getModelMatrix(), camera->getView(), projection);
    // cubes
    cube.bindVertexArray();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dudvMap);
    cube.render();

    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, dudvMap);
    shader.setMatrix4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    boatShader.activate();
    boatShader.setMatrix4("view", camera->getView());
    boatShader.setMatrix4("projection", projection);
    boatShader.setVector4("clippingPlane", clippingPlane);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(400.0f, 2.0f, 700.0f));
    boatShader.setMatrix4("model", model);
    boatModel.render();
}

void Renderer::renderRefractionPass()
{
    terrainShader.activate();
    glm::vec4 clippingPlane = glm::vec4(0.f, -1.f, 0.f, 6.f);
    terrainShader.setVector4("clippingPlane", clippingPlane);
    terrainShader.setMatrix4("view", camera->getView());
    terrainShader.setVector3("lightColour", light.getLightColour());
    terrainShader.setMatrix4("model", terrainModel);
    renderOpaqueObjects(clippingPlane);
}

void Renderer::renderReflectionPass()
{
    glEnable(GL_CLIP_DISTANCE0);
    glm::vec4 clippingPlane = glm::vec4(0.f, 1.f, 0.f, -6.f);
    terrainShader.activate();
    terrainShader.setMatrix4("view", camera->getView());
    terrainShader.setVector3("lightPosition", glm::vec3(1,1,0));
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
    waterShader.setVector3("lightPosition", light.getLightPosition());
    waterShader.setVector3("lightColour", light.getLightColour());
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

