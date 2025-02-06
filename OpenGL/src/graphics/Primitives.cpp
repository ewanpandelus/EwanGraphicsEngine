#include "Primitives.h"
#include <GL/glew.h>


//// BASE PRIMITIVE ////


Primitive::Primitive(glm::vec3 initialPosition, float scale)
{
    modelMatrix = glm::translate(modelMatrix, initialPosition);
    m_scale = scale;
}


//// CUBE ////

Cube::Cube(glm::vec3 initialPosition, float scale) :  Primitive(initialPosition, scale)
{

}

void Cube::generateBuffers()
{
   float VERTICES[] = 
   {
        -m_scale,  m_scale, -m_scale,
        -m_scale, -m_scale, -m_scale,
        m_scale, -m_scale, -m_scale,
         m_scale, -m_scale, -m_scale,
         m_scale,  m_scale, -m_scale,
        -m_scale,  m_scale, -m_scale,


        -m_scale, -m_scale,  m_scale,
        -m_scale, -m_scale, -m_scale,
        -m_scale,  m_scale, -m_scale,
        -m_scale,  m_scale, -m_scale,
        -m_scale,  m_scale,  m_scale,
        -m_scale, -m_scale,  m_scale,


         m_scale, -m_scale, -m_scale,
         m_scale, -m_scale,  m_scale,
         m_scale,  m_scale,  m_scale,
         m_scale,  m_scale,  m_scale,
         m_scale,  m_scale, -m_scale,
         m_scale, -m_scale, -m_scale,


        -m_scale, -m_scale,  m_scale,
        -m_scale,  m_scale,  m_scale,
         m_scale,  m_scale,  m_scale,
         m_scale,  m_scale,  m_scale,
         m_scale, -m_scale,  m_scale,
        -m_scale, -m_scale,  m_scale,


        -m_scale,  m_scale, -m_scale,
         m_scale,  m_scale, -m_scale,
         m_scale,  m_scale,  m_scale,
         m_scale,  m_scale,  m_scale,
        -m_scale,  m_scale,  m_scale,
        -m_scale,  m_scale, -m_scale,


        -m_scale, -m_scale, -m_scale,
        -m_scale, -m_scale,  m_scale,
         m_scale, -m_scale, -m_scale,
         m_scale, -m_scale, -m_scale,
        -m_scale, -m_scale,  m_scale,
         m_scale, -m_scale,  m_scale
    };

    // float vertices[] = {
    //     //positions          // texture Coords
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    //};

    glGenVertexArrays(1, &primitiveVAO);
    glGenBuffers(1, &primitiveVBO);
    glBindVertexArray(primitiveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, primitiveVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), &VERTICES, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Cube::bindVertexArray()
{
    glBindVertexArray(primitiveVAO);
}

void Cube::render()
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


