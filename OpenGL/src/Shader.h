#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    // activate the shader
    // 
    void initialise(const char* vertexPath, const char* fragmentPath);
    // ------------------------------------------------------------------------
    void activate();
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value); const

     // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const;

    void setVector3(const std::string& name, glm::vec3 value) const;

    void setVector4(const std::string& name, glm::vec4 value) const;

    void setMatrix4(const std::string& name, glm::mat4 value) const;
protected:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
