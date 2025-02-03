#define STB_IMAGE_IMPLEMENTATION
#include "Terrain.h"
#include "../maths/PerlinNoise.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>

Terrain::Terrain(int resolution, float scale, glm::vec2 offset)
{
	m_scale = scale;
	m_resolution = resolution;
	m_offset = offset;
	terrainModel = glm::translate(terrainModel, glm::vec3(offset.x, -500, offset.y));
	initialiseHeightMap();
	calculateNormals();
	generateBuffers();
	setupBufferData();
}

Terrain::~Terrain() 
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO); 
	vertices.clear();
	indices.clear();
}

void Terrain::initialiseHeightMap()
{
    m_heightMap = new HeightMapType[m_resolution * m_resolution];

	if (!m_heightMap)
	{
		return;
	}
    float textureCoordinatesStep = m_resolution *  2;
    int index;
	PerlinNoise pn;
	pn.Initialize();
	float initialAmp = m_amplitude;
	float initialFrequency = m_frequency;
	int centrePos = m_resolution / 2;
    for (int j = 0; j < m_resolution; j++)
    {
        for (int i = 0; i < m_resolution; i++)
        {
            index = (m_resolution * j) + i;
			glm::vec2 percent = glm::vec2(j / ((float) m_resolution - 1), i / ((float) m_resolution - 1));


			float noiseHeight = 0;
			for (int octave = 0; octave < m_octaves; octave++) 
			{
				float perlinValue = pn.Evaluate(glm::vec3(i,j,1), m_frequency) * m_amplitude;
			    noiseHeight += perlinValue;
				m_amplitude *= m_persistence;
				m_frequency *= m_lacunarity;
			}
			const float distanceToCentre = (glm::distance(glm::vec2(i, j), glm::vec2(centrePos, centrePos)));
	
			noiseHeight += distanceToCentre / 2.5f;
			

			
	
			m_amplitude = initialAmp;
			m_frequency = initialFrequency;
	
			
	
		

			m_heightMap[index].x = i*m_scale;
			m_heightMap[index].y = noiseHeight;
			m_heightMap[index].z = j*m_scale;

            //and use this step to calculate the texture coordinates for this point on the terrain.
            m_heightMap[index].u = (float)i / textureCoordinatesStep;
            m_heightMap[index].v = (float)j / textureCoordinatesStep;


        }
    }
}

void Terrain::setupBufferData() {
	int i, j;
	int index1, index2, index3, index4;

	// Clear and resize buffers
	vertices.clear();
	indices.clear();

	vertices.reserve(m_resolution * m_resolution * 8);
	indices.reserve((m_resolution - 1) * (m_resolution - 1) * 6);

	int currentIndex = 0;

	for (j = 0; j < (m_resolution - 1); j++) {
		for (i = 0; i < (m_resolution - 1); i++) {
			// Compute indices in heightmap
			index1 = (m_resolution * j) + i;             // Bottom-left
			index2 = (m_resolution * j) + (i + 1);       // Bottom-right
			index3 = (m_resolution * (j + 1)) + i;       // Top-left
			index4 = (m_resolution * (j + 1)) + (i + 1); // Top-right

			// Ensure indices are within bounds
			if (index1 >= m_resolution * m_resolution || index2 >= m_resolution * m_resolution ||
				index3 >= m_resolution * m_resolution || index4 >= m_resolution * m_resolution) {
				continue;
			}

			// Zig-zag alternating split pattern
			if ((i + j) % 2 == 0) {
				// First Triangle: Top-left, Bottom-right, Bottom-left
				SetupVertex(&currentIndex, index3);
				SetupVertex(&currentIndex, index4);
				SetupVertex(&currentIndex, index1);

				// Second Triangle: Bottom-left, Bottom-right, Top-right
				SetupVertex(&currentIndex, index1);
				SetupVertex(&currentIndex, index4);
				SetupVertex(&currentIndex, index2);
			}
			else {
				// Alternate split direction for better shading
				// First Triangle: Top-left, Top-right, Bottom-left
				SetupVertex(&currentIndex, index3);
				SetupVertex(&currentIndex, index1);
				SetupVertex(&currentIndex, index4);

				// Second Triangle: Bottom-left, Top-right, Bottom-right
				SetupVertex(&currentIndex, index1);
				SetupVertex(&currentIndex, index2);
				SetupVertex(&currentIndex, index4);
			}
		}
	}
}


void Terrain::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Terrain::calculateNormals() {
	// Loop through all vertices in the heightmap
	for (int j = 0; j < m_resolution; j++) {
		for (int i = 0; i < m_resolution; i++) {
			// Get the center index
			int index = (j * m_resolution) + i;

			// Sample neighbors using central difference method
			int left = (i > 0) ? ((j * m_resolution) + (i - 1)) : index;
			int right = (i < m_resolution - 1) ? ((j * m_resolution) + (i + 1)) : index;
			int down = (j > 0) ? (((j - 1) * m_resolution) + i) : index;
			int up = (j < m_resolution - 1) ? (((j + 1) * m_resolution) + i) : index;

			// Get positions of neighboring points
			glm::vec3 pL(m_heightMap[left].x, m_heightMap[left].y, m_heightMap[left].z);
			glm::vec3 pR(m_heightMap[right].x, m_heightMap[right].y, m_heightMap[right].z);
			glm::vec3 pD(m_heightMap[down].x, m_heightMap[down].y, m_heightMap[down].z);
			glm::vec3 pU(m_heightMap[up].x, m_heightMap[up].y, m_heightMap[up].z);

			// Compute gradient vectors
			glm::vec3 dx = pR - pL;
			glm::vec3 dz = pU - pD;

			// Compute normal using cross product (taking terrain slope into account)
			glm::vec3 normal = glm::normalize(glm::cross(dz, dx));

			// Assign the computed normal
			m_heightMap[index].nx = normal.x;
			m_heightMap[index].ny = normal.y;
			m_heightMap[index].nz = normal.z;
		}
	}
}


void Terrain::bindBuffers() {

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Terrain::render() 
{
	bindBuffers();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	vertices.clear();
	indices.clear();
}

void Terrain::SetupVertex(int* currentIndex, int vertexIndex) {
	if (vertexIndex < 0 || vertexIndex >= m_resolution * m_resolution) {
		return; // Avoid out-of-bounds memory access
	}

	// Push position
	vertices.push_back(m_heightMap[vertexIndex].x);
	vertices.push_back(m_heightMap[vertexIndex].y);
	vertices.push_back(m_heightMap[vertexIndex].z);

	// Push normal
	vertices.push_back(m_heightMap[vertexIndex].nx);
	vertices.push_back(m_heightMap[vertexIndex].ny);
	vertices.push_back(m_heightMap[vertexIndex].nz);

	// Push texture coordinates
	vertices.push_back(m_heightMap[vertexIndex].u);
	vertices.push_back(m_heightMap[vertexIndex].v);

	// Store index safely
	if (*currentIndex < indices.capacity()) {
		indices.push_back(*currentIndex);
	}

	(*currentIndex)++;
}