#pragma once
#include "Terrain.h"
#include "water/Water.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "water/WaterFrameBuffers.h"
#include "graphics/Shader.h"
#include "graphics/Light.h"
#include "graphics/Renderer.h"
#include "graphics/TextureLoader.h"
#include "graphics/Model.h"

class Game
{
public:
    Game();

    void initialise(int width, int height, GLFWwindow* window);
    
    void tick(GLFWwindow* window);

    Camera* getCamera()  {return &camera;}

private:
    void render();

    void cleanUp();

    // Rendering
    Renderer renderer;

    // Camera 
    Camera camera;

    // Input 
    InputManager inputManager;

    // Timing 
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float totalTime = 0.0f; // Total elapsed time of game

    // Transformation Matrices 
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
       
    // Shaders
    Shader terrainShader; 
    Shader treeShader;
    Shader screenShader;
    Shader shader;
    Shader waterShader;

    // Lighting 
    Light light; 

    // Textures
    unsigned int cubeTexture;
    unsigned int floorTexture;
    
    // FrameBufferObjects 
    WaterFrameBuffers waterFrameBuffers;

    // Environment
    Terrain terrain;
    Water water;
    Model monkeyModel;
    Model tree;
   
    // Environment Model Matrices
    glm::mat4 terrainModel = glm::mat4(1.0f);
    glm::mat4 waterModel = glm::mat4(1.0f);
    glm::mat4 treeModel = glm::mat4(1.0f);

    std::vector<glm::vec3> treePositions;

    // --------------------------------------- TEMP - TO CLEAN

    float quadVertices[24];// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    unsigned int quadVAO, quadVBO;
    //
};

