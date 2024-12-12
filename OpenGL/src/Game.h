#pragma once
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"
#include "Terrain.h"
#include "water/Water.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "water/WaterFrameBuffers.h"
#include "Light.h"

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
   
};

