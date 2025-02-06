#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Light
{
public:
    Light(glm::vec3 _lightPosition, glm::vec3 _lightColour) 
    {
        lightPosition = _lightPosition;
        lightColour = _lightColour;
    }

    Light() = default;

    void SetLightPosition(glm::vec3 _lightPosition) { lightPosition = _lightPosition; }
    void SetLightColour(glm::vec3 _lightColour) { lightColour = _lightColour; }


    glm::vec3 getLightPosition() {return lightPosition;}
    glm::vec3 getLightColour() {return lightColour; }

protected:
    glm::vec3 lightPosition{0};
    glm::vec3 lightColour{ 1.0f,1.0f,1.0f};
};
