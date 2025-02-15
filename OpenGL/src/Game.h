#pragma once
#include "terrain/Terrain.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "water/WaterFrameBuffers.h"
#include "graphics/Renderer.h"


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
    
    // FrameBufferObjects 
    WaterFrameBuffers waterFrameBuffers;
};

