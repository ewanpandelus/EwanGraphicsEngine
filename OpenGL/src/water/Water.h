#pragma once
#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>

class Water
{
public:
    Water(int resolution, float scale);
    Water() = default;
    ~Water();
    void render();
    void clean();

    struct VertexPositionMapType
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    glm::vec3 GetPosition() { return m_position; }

private:
    void initialiseHeightMap();
    void setupBufferData();
    void generateBuffers();
    void bindBuffers();
    void SetupVertex(int* currentIndex, int triangle1Index);
    VertexPositionMapType* m_vertexPositionMap;

    int m_resolution;
    float m_scale;

    glm::vec3 m_position = glm::vec3(0, 6, 0);

    std::vector<std::vector<float>> vertexMap;
    std::vector<float> vertices;
    std::vector<long> indices;
    int m_vertexCount, m_indexCount;

    GLuint VAO, VBO, EBO;
};

