#pragma once
#include <GL/glew.h>
class WaterFrameBuffers
{

protected:

	const static int REFLECTION_WIDTH = 320;
    const static int REFLECTION_HEIGHT = 180;
	const static int REFRACTION_WIDTH = 1280;
	const static int REFRACTION_HEIGHT = 720;

public:
	WaterFrameBuffers();
	
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

	 int createFrameBuffer();
	
	 int createTextureAttachment(int width, int height);

	 int createDepthTextureAttachment(int width, int height);

	 int createDepthBufferAttachment(int width, int height);

	 GLuint reflectionFrameBuffer;
	 GLuint reflectionTexture;
	 GLuint reflectionDepthBuffer;

	 GLuint refractionFrameBuffer;
	 GLuint refractionTexture;
	 GLuint refractionDepthTexture;


	 GLuint frameBuffer;
	 GLuint texture;
	 GLuint depthTexture;
	 GLuint depthBuffer;

};

