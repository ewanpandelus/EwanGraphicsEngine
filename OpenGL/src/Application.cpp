#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "InputManager.h"
#include "Renderer.h"
#include "Model.h"
#include "Planet.h"
#include "water/Water.h"
#include "water/WaterFrameBuffers.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Camera camera;



void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() 
{
 
    GLFWwindow* window;
    InputManager inputManager;
    Light light(glm::vec3(10.0f,10.0f, 10.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f));

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame




    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);;


    std::vector<glm::vec3> treePositions;
    treePositions.push_back(glm::vec3(5, 5, 100));
    treePositions.push_back(glm::vec3(100, 5, 5));
    treePositions.push_back(glm::vec3(100, 5, 100));
    treePositions.push_back(glm::vec3(50, 5, 50));
    treePositions.push_back(glm::vec3(30, 5, 15));

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
  
    int width, height;

    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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
  
    Renderer renderer;
    Shader terrainShader("src/shaders/vsTerrainShader.glsl", "src/shaders/fsTerrainShader.glsl");
    terrainShader.activate();
    terrainShader.setMatrix4("projection", projection);


    Shader waterShader("src/shaders/vsWaterShader.glsl", "src/shaders/fsWaterShader.glsl");
    waterShader.activate();
    waterShader.setMatrix4("projection", projection);



    Shader treeShader("src/shaders/vsTreeShader.glsl", "src/shaders/fsTreeShader.glsl");
    treeShader.activate();
    treeShader.setMatrix4("projection", projection);


    Terrain terrain(glm::vec3(0, 1, 0), 256, 0.5f);
    Water water(256, 0.5f);
    Model monkeyModel;
    monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/Crate.png");


    Model tree;
    tree.prepareModel("resources/objects/tree.obj", "resources/textures/Crate.png");

    glm::mat4 terrainModel = glm::mat4(1.0f);

    glm::mat4 waterModel = glm::mat4(1.0f);
    waterModel = glm::translate(waterModel, water.GetPosition());

    glm::mat4 treeModel = glm::mat4(1.0f);

    //WaterFrameBuffers fbos = WaterFrameBuffers();
    //Model monkeyModel;
    //monkeyModel.prepareModel("resources/objects/monkey.obj", "resources/textures/Crate.png");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    // build and compile shaders
    // -------------------------
    Shader shader("src/shaders/vsStandard.glsl", "src/shaders/fsStandard.glsl");
    Shader screenShader("src/shaders/vsScreen.glsl","src/shaders/fsScreen.glsl" );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
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
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    float totalTime = 0;
    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("resources/textures/container.png");
    unsigned int floorTexture = loadTexture("resources/textures/wall.png");

    // shader configuration
    // --------------------
    shader.activate();
    shader.setInt("texture1", 0);

    screenShader.activate();
    screenShader.setInt("screenTexture", 0);
    WaterFrameBuffers fbos;
   
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);




        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        totalTime += deltaTime;
        
        inputManager.updateInputCommands(window);
        camera.updateCameraPosition(&inputManager, deltaTime);


        glm::vec3 cameraPos = camera.getCameraPos();
        float distance = 2 * (cameraPos.y - water.GetPosition().y);
        cameraPos.y -= distance;
        camera.setCameraPos(cameraPos);
        camera.invertPitch();

        // bind to framebuffer and draw scene as we normally would to color texture REFLECTION RENDER

        fbos.bindReflectionFrameBuffer();
        glEnable(GL_DEPTH_TEST); 

        // make sure we clear the framebuffer's content
        renderer.prepare();

        glm::mat4 view;
        view = camera.getView();
        shader.activate();
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMatrix4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

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

        fbos.unbindCurrentFrameBuffer();
        fbos.bindRefractionFrameBuffer();
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
        glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());	// use the color attachment texture as the texture of the quad plane
        terrain.render();


        fbos.unbindCurrentFrameBuffer();
        renderer.prepare();

        //// Screen space quad with frame buffer texture 
         screenShader.activate();
        glBindVertexArray(quadVAO);
        if (fmod(floor(totalTime), 2) == 1) 
        {
            glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());	// use the color attachment texture as the texture of the quad plane
        }
        else 
        {
            glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());	// use the color attachment texture as the texture of the quad plane
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

        
    
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.updateCameraOrientation(xpos, ypos);
}
