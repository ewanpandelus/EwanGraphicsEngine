#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



struct Vertex {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texcoord[2];
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint vao, vbo, ebo;
};

class Model {

public:
    void prepareModel(const std::string& objFilepath, const char* textureFilePath);

    void loadOBJ(const std::string& filepath);

    void loadTexture(const char* filepath);

    void bindTexture();

    void createBuffers();

    void render();
    
    void clean();

private:
    Mesh mesh;
    unsigned int texture;
    unsigned char* data;
    int width, height;
};


