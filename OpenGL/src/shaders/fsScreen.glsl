#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    float near = 0.1; 
    float far  = 1000.0; 
    float depth = texture(screenTexture, TexCoords).r;
    float linearDepth = (2.0 * near * far) / (far + near - (2.0 * depth - 1.0) * (far - near));

    float eye_z = near * far / ((depth * (far - near)) - far);
    float val = ( eye_z - (-near) ) / ( -far - (-near) );
    FragColor = vec4(val, val, val, 1);
}
