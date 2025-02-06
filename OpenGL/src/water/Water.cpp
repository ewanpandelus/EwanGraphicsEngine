#include "Water.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

Water::Water(int resolution, float scale)
{
	m_scale = scale;
	m_resolution = resolution;
	initialiseHeightMap();
	generateBuffers();
	setupBufferData();
}

Water::~Water()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	vertices.clear();
	indices.clear();
}

void Water::initialiseHeightMap()
{
	m_positionMap = new PositionMapType[m_resolution * m_resolution];

	if (!m_positionMap)
	{
		return;
	}
	float textureCoordinatesStep = m_resolution * 2;
	int index;
	for (int j = 0; j < m_resolution; j++)
	{
		for (int i = 0; i < m_resolution; i++)
		{
			index = (m_resolution * j) + i;
	



			m_positionMap[index].x = i * m_scale;
			m_positionMap[index].y = 0;
			m_positionMap[index].z = j * m_scale;

			//and use this step to calculate the texture coordinates for this point on the terrain.
			m_positionMap[index].u = (float)i / textureCoordinatesStep;
			m_positionMap[index].v = (float)j / textureCoordinatesStep;

			m_positionMap[index].nx = 0;
			m_positionMap[index].ny = 1;
			m_positionMap[index].nz = 0;
		}
	}
}

void Water::setupBufferData()
{
	int index, i, j;
	int index1, index2, index3, index4; //geometric indices. 

	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_resolution - 1) * (m_resolution - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the index array.
	indices.resize(m_indexCount);// = new unsigned long[m_indexCount];

	// Initialize the index to the vertex buffer.
	index = 0;
	int counter = 0;

	for (j = 0; j < (m_resolution - 1); j++)
	{
		for (i = 0; i < (m_resolution - 1); i++)
		{

			if (counter % 2 == 0) {

				index1 = (m_resolution * j) + i;             //Bottom left
				index2 = (m_resolution * j) + (i + 1);      // Bottom right
				index3 = (m_resolution * (j + 1)) + i;       //Upper left 
				index4 = (m_resolution * (j + 1)) + (i + 1); //Upper right 
			}
			else {
				index1 = (m_resolution * (j + 1)) + i;       //Upper left 
				index3 = (m_resolution * (j + 1)) + (i + 1); //Upper right 
				index2 = (m_resolution * j) + i;             //Bottom left
				index4 = (m_resolution * j) + (i + 1);      // Bottom right

			}
			counter++;

			SetupVertex(&index, index3);

			// Upper right.
			SetupVertex(&index, index4);

			// Bottom left.
			SetupVertex(&index, index1);

			// Bottom left.
			SetupVertex(&index, index1);

			// Upper right.
			SetupVertex(&index, index4);

			// Bottom right.
			SetupVertex(&index, index2);

		}
	}
}

void Water::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Water::bindBuffers() {

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

void Water::render()
{
	bindBuffers();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Water::clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	vertices.clear();
	indices.clear();
}

void Water::SetupVertex(int* currentIndex, int triangle1Index)
{
	int index = *currentIndex;

	//position
	vertices.push_back(m_positionMap[triangle1Index].x);
	vertices.push_back(m_positionMap[triangle1Index].y);
	vertices.push_back(m_positionMap[triangle1Index].z);

	//normal
	vertices.push_back(m_positionMap[triangle1Index].nx);
	vertices.push_back(m_positionMap[triangle1Index].ny);
	vertices.push_back(m_positionMap[triangle1Index].nz);

	//tex coords 
	vertices.push_back(m_positionMap[triangle1Index].u);
	vertices.push_back(m_positionMap[triangle1Index].v);

	indices[index] = index;
	*currentIndex += 1;
}