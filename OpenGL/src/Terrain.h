#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include <string>




class Terrain {
public:
    Terrain(glm::vec3 localUp, int resolution, float scale);
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

    float m_frequency = 0.05f, m_amplitude = 0.1f;
    float m_persistence = 0.4f, m_lacunarity = 2.5f;


    int m_octaves = 5;

    int m_vertexCount, m_indexCount;

    GLuint VAO, VBO, EBO;
};

#endif