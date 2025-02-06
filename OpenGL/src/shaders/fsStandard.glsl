#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
    FragColor = vec4(0.7, 0.2,0.2,1); // texture(texture1, TexCoords);

}