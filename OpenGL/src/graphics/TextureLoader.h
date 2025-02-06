#pragma once
#include <vector>
#include <string>

class TextureLoader
{
public:
	static unsigned int loadTexture(std::string);
	
	static unsigned int loadCubeMapTexture(std::vector<std::string> paths);


};

