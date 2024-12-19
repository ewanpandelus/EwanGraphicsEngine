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
    light = Light(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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


    terrain = new Terrain(glm::vec3(0, 1, 0), 256, 4);
    water = new Water(256, 4);
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/Crate.png");
    Model tree;
    tree.prepareModel("resources/objects/tree.obj", "resources/textures/Crate.png");

    terrainModel = glm::translate(waterModel, glm::vec3(0, -430, 0));
    waterModel = glm::translate(waterModel, glm::vec3(25,6,25));

    shader.activate();
    shader.setInt("texture1", 0);

    screenShader.activate();
    screenShader.setInt("screenTexture", 0);

    unsigned int cubeTexture = TextureLoader::loadTexture("resources/textures/container.png");
    unsigned int floorTexture = TextureLoader::loadTexture("resources/textures/wall.png");


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

void Renderer::renderOpaqueObjects()
{
    terrainShader.activate();
    terrainShader.setMatrix4("model", glm::mat4(1.0f));
    terrainShader.setMatrix4("view", m_currentView);
    terrainShader.setVector3("lightPosition", light.getLightPosition());
    terrainShader.setVector4("lightColour", light.getLightColour());
    terrainShader.setMatrix4("model", terrainModel);
    terrain->render();

    treeShader.activate();
    treeShader.setMatrix4("model", glm::mat4(1.0f));
    treeShader.setMatrix4("view", m_currentView);
    treeShader.setVector3("lightPosition", light.getLightPosition());
    treeShader.setVector4("lightColour", light.getLightColour());
    treeShader.setMatrix4("model", treeModel);
    tree.render();
}

void Renderer::renderRefractionPass()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    terrainShader.activate();
    // maybe not working properly?
    // isn't working
    terrainShader.setVector4("plane", glm::vec4(0,-1,0,6));
    renderOpaqueObjects();
}

void Renderer::renderReflectionPass()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    terrainShader.activate();
    terrainShader.setVector4("plane", glm::vec4(0, 1, 0, -6));
    renderOpaqueObjects();
}

void Renderer::renderWater(unsigned int reflectionTexture, unsigned int refractionTexture)
{
    waterShader.activate();
    waterShader.setMatrix4("model", glm::mat4(1.0f));
    waterShader.setMatrix4("view", m_currentView);
    waterShader.setVector3("lightPosition", light.getLightPosition());
    waterShader.setVector4("lightColour", light.getLightColour());
    waterShader.setMatrix4("model", waterModel);
    glDepthMask(false); //disable z-testing
    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water->render();
    glDepthMask(true); //disable z-testing
    glDisable(GL_BLEND);
}

void Renderer::renderToScreen(unsigned int textureToBind)
{
    screenShader.activate();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureToBind);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

