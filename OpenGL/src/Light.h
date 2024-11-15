#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Light
{
public:
    Light(glm::vec3 _lightPosition, glm::vec4 _lightColour) 
    {
        lightPosition = _lightPosition;
        lightColour = _lightColour;
    }


    void SetLightPosition(glm::vec3 _lightPosition) { lightPosition = _lightPosition; }
    void SetLightColour(glm::vec4 _lightColour) { lightColour = _lightColour; }


    glm::vec3 getLightPosition() {return lightPosition;}
    glm::vec4 getLightColour() {return lightColour; }

protected:
    glm::vec3 lightPosition{0};
    glm::vec4 lightColour{ 1.0f,1.0f,1.0f,0.0f};
};
