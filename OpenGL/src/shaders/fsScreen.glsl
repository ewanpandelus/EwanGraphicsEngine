#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    float near = 0.1; 
    float far  = 1000.0; 

    

    float depth = texture(screenTexture, TexCoords).r;

    FragColor =  1 - texture(screenTexture, TexCoords);
}
