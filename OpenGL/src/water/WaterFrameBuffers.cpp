#include "WaterFrameBuffers.h"


WaterFrameBuffers::WaterFrameBuffers()
{
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();

}

void WaterFrameBuffers::cleanUp()
{
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);

}

void WaterFrameBuffers::bindReflectionFrameBuffer()
{
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
	bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080); // Create a variable for width, height. Pass into water class
}

int WaterFrameBuffers::getReflectionTexture()
{
	return reflectionTexture;
}

int WaterFrameBuffers::getRefractionTexture()
{
	return refractionTexture;
}

int WaterFrameBuffers::getRefractionDepthTexture()
{
	return refractionDepthTexture;
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer()
{
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer()
{
	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);

}

GLuint WaterFrameBuffers::createFrameBuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frameBuffer;

}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned char* data{};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		texture, 0);
	return texture;

}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	unsigned char* data{};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
		depthTexture, 0);
	return depthTexture;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
		height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

