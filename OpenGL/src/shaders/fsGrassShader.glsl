#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 tryColor;
uniform vec4 lightColour;
uniform sampler2D ourTexture;

void main()
{
    vec4 texColour = texture(ourTexture, TexCoord);
    vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(toLightVector);
    float diff = max(dot(norm, lightDir), 0.0);

    vec4 diffuse = diff * lightColour;

    vec4 result = (ambient + diffuse);
    result.a = texColour.a;

    FragColor = result;
}