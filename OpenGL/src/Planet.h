#pragma once
#include <vector>
#include "Terrain.h"
class Planet
{
public:
	Planet(int resolution, int scale);

	void constructMeshes();

	void render();

	void cleanUp();
	
private:
	std::vector<Terrain> terrainFaces;

	int m_resolution;
	float m_scale;
};

