//#include "Planet.h"
//
//Planet::Planet(int resolution, int scale)
//{
//	m_resolution = resolution;
//	m_scale = scale;
//	
//}
//
//void Planet::constructMeshes()
//{
//	glm::vec3 directions[] = { glm::vec3(0,1,0), glm::vec3(0,-1,0),
//						   glm::vec3(-1,0,0), glm::vec3(1,0,0),
//						   glm::vec3(0,0,1), glm::vec3(0,0,-1) };
//	for (int i = 0; i < 6; i++)
//	{
//		Terrain terrainFace(directions[i], m_resolution, m_scale);
//		terrainFaces.push_back(terrainFace);
//	}
//
//	
//}
//
//void Planet::render()
//{
//	glm::vec3 directions[] = { glm::vec3(0,1,0), glm::vec3(0,-1,0),
//						   glm::vec3(-1,0,0), glm::vec3(1,0,0),
//						   glm::vec3(0,0,1), glm::vec3(0,0,-1) };
//	
//	for(Terrain& terrainFace : terrainFaces)
//	{
//		terrainFace.render();
//	}
//}
//
//void Planet::cleanUp()
//{
//	for (Terrain terrainFace : terrainFaces)
//	{
//		terrainFace.clean();
//	}
//}
