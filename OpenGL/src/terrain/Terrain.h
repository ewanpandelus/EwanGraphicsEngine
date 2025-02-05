#pragma once



#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>




class Terrain {
public:
    Terrain(int resolution, float scale, glm::vec2 offset);

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

   glm::mat4* GetModelMatrix() { return &terrainModel; }

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

    glm::mat4 terrainModel = glm::mat4(1.0f);

    std::vector<std::vector<float>> heightmap;
    std::vector<float> vertices;
    std::vector<long> indices;

    float m_frequency = 0.25f, m_amplitude = 18.f;
    float m_persistence = 2.4f, m_lacunarity = 0.45f;

    glm::vec2 m_offset;
 
    int m_octaves = 5;

    int m_vertexCount, m_indexCount;

    GLuint VAO = 0, VBO = 0, EBO = 0;
};

