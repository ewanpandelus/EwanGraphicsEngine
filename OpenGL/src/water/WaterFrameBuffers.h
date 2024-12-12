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

	 const unsigned int SCR_WIDTH = 800;
	 const unsigned int SCR_HEIGHT = 600;
};

