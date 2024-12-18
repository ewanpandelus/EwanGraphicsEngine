#pragma once



#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>




class Terrain {
public:
    Terrain(glm::vec3 localUp, int resolution, float scale);
    Terrain() = default;
    ~Terrain();
    void render();
    void clean();
    
    struct HeightMapType
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
   
    void calculateNormals();
    HeightMapType* m_heightMap;

    int m_resolution;
    float m_scale;


    glm::vec3 m_localUp;
    glm::vec3 m_axisA;
    glm::vec3 m_axisB;

    std::vector<std::vector<float>> heightmap;
    std::vector<float> vertices;
    std::vector<long> indices;

    float m_frequency = 0.1f, m_amplitude = 20.f;
    float m_persistence = 2.f, m_lacunarity = 0.45f;
    
    float m_maximumPondDepth = 20;

    int m_octaves = 5;

    int m_vertexCount, m_indexCount;

    GLuint VAO = 0, VBO = 0, EBO = 0;
};

