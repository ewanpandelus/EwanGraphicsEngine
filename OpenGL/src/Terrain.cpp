#define STB_IMAGE_IMPLEMENTATION
#include "Terrain.h"
#include "maths/PerlinNoise.h"
#include <iostream>

Terrain::Terrain(glm::vec3 localUp, int resolution, float scale)
{
	m_localUp = localUp;
	m_axisA =  glm::vec3(m_localUp.y, m_localUp.z, m_localUp.x);
	m_axisB = glm::cross(m_localUp, m_axisA);
	m_scale = scale;
	m_resolution = resolution;

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
	
			noiseHeight += distanceToCentre;
			

			
	
			m_amplitude = initialAmp;
			m_frequency = initialFrequency;
	
			
	
		

			m_heightMap[index].x = i*m_scale;
			m_heightMap[index].y = noiseHeight;
			m_heightMap[index].z = j*m_scale;

            //and use this step to calculate the texture coordinates for this point on the terrain.
            m_heightMap[index].u = (float)i / textureCoordinatesStep;
            m_heightMap[index].v = (float)j / textureCoordinatesStep;

            m_heightMap[index].nx = 0;
            m_heightMap[index].ny = 0;
            m_heightMap[index].nz = 0;
        }
    }
}

void Terrain::setupBufferData()
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

void Terrain::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Terrain::calculateNormals() {
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	glm::vec3* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new glm::vec3 [(m_resolution - 1) * (m_resolution - 1)];
	if (!normals)
	{
		return;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_resolution - 1); j++)
	{
		for (i = 0; i < (m_resolution - 1); i++)
		{
			index1 = (j * m_resolution) + i;
			index2 = (j * m_resolution) + (i + 1);
			index3 = ((j + 1) * m_resolution) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_resolution - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j < m_resolution; j++)
	{
		for (i = 0; i < m_resolution; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_resolution - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_resolution - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_resolution - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_resolution - 1)))
			{
				index = (j * (m_resolution - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_resolution - 1)) && (j < (m_resolution - 1)))
			{
				index = (j * (m_resolution - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_resolution) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;
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

void Terrain::SetupVertex(int* currentIndex, int triangle1Index)
{
    int index = *currentIndex;

	//position
	vertices.push_back(m_heightMap[triangle1Index].x);
	vertices.push_back(m_heightMap[triangle1Index].y);
	vertices.push_back(m_heightMap[triangle1Index].z);

	//normal
	vertices.push_back(m_heightMap[triangle1Index].nx);
	vertices.push_back(m_heightMap[triangle1Index].ny);
	vertices.push_back(m_heightMap[triangle1Index].nz);

	//tex coords 
	vertices.push_back(m_heightMap[triangle1Index].u);
	vertices.push_back(m_heightMap[triangle1Index].v);

    indices[index] = index;
    *currentIndex += 1;
}