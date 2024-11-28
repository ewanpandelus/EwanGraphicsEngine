#pragma once
#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>
#include "../Shader.h"

class Water
{
public:
    Water(int resolution, float scale);
    ~Water();
    void render();
    void clean();

    struct VertexPositionMapType
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    Shader getShader() { return waterShader;}

private:

    void initialiseHeightMap();
    void setupBufferData();
    void generateBuffers();
    void bindBuffers();
    void SetupVertex(int* currentIndex, int triangle1Index);
    VertexPositionMapType* m_vertexPositionMap;

    int m_resolution;
    float m_scale;


    std::vector<std::vector<float>> vertexMap;
    std::vector<float> vertices;
    std::vector<long> indices;
    int m_vertexCount, m_indexCount;
    GLuint VAO, VBO, EBO;

    Shader waterShader{};
};

