#pragma once



#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>




class Water {
public:
    Water(int resolution, float scale);
    Water() = default;
    ~Water();
    void render();
    void clean();
    glm::vec3 getPosition (){ return m_position; }

    struct PositionMapType
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

private:

    void initialiseHeightMap();
    void setupBufferData();
    void generateBuffers();
    void bindBuffers();
    void SetupVertex(int* currentIndex, int triangle1Index);


    PositionMapType* m_positionMap;

    int m_resolution;
    float m_scale;

    glm::vec3 m_position = glm::vec3(0, 6, 0);

    std::vector<std::vector<float>> positionMap;
    std::vector<float> vertices;
    std::vector<long> indices;

    float m_frequency = 0.05f, m_amplitude = 6.f;
    float m_persistence = 0.5f, m_lacunarity = 0.2f;


    int m_octaves = 5;

    int m_vertexCount, m_indexCount;

    GLuint VAO = 0, VBO = 0, EBO = 0;
};

