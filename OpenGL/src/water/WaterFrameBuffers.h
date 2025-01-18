#pragma once
#include <GL/glew.h>
class WaterFrameBuffers
{
public:
	void initialise();
	
	void cleanUp();

	void bindReflectionFrameBuffer();

	void bindRefractionFrameBuffer(); 
	
	void unbindCurrentFrameBuffer();
	
	int getReflectionTexture();

	int getRefractionTexture();

	int getRefractionDepthTexture();

private:
	 void initialiseReflectionFrameBuffer(); 
	
	 void initialiseRefractionFrameBuffer();

	 void bindFrameBuffer(int frameBuffer, int width, int height);

	 GLuint createFrameBuffer();
	
	 GLuint createTextureAttachment(int width, int height);

	 GLuint createDepthTextureAttachment(int width, int height);

	 GLuint createDepthBufferAttachment(int width, int height);

	 GLuint reflectionFrameBuffer;
	 GLuint reflectionTexture;
	 GLuint reflectionDepthBuffer;

	 GLuint refractionFrameBuffer;
	 GLuint refractionTexture;
	 GLuint refractionDepthTexture;


	 GLuint frameBuffer;
	 GLuint depthTexture;
	 GLuint depthBuffer;



	 GLuint textureColorbuffer;
	 GLuint renderBufferObject;

	 const unsigned int SCR_WIDTH = 1920;
	 const unsigned int SCR_HEIGHT = 1080;
};

