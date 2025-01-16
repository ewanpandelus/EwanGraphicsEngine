#pragma once
#include <vector>
#include "Shader.h"
class Primitive
{ 
public:
    Primitive(glm::vec3 initialPosition);

    virtual void generateBuffers() = 0;

    virtual void bindVertexArray() = 0;

    virtual void render() = 0;

    Shader* getShader() { return &primitiveShader;}

    glm::mat4* getModelMatrix(){ return &modelMatrix;}

protected:
	unsigned int primitiveVAO = 0, primitiveVBO = 0;

    glm::mat4 modelMatrix  = glm::mat4(1);

    Shader primitiveShader;
};


class Cube : public Primitive
{
public:
    Cube(glm::vec3 initialPosition);
    
    virtual void generateBuffers();

    virtual void bindVertexArray();

    virtual void render();
};

