#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std430, binding = 0) buffer GrassBuffer {
    vec4 grassPositions[]; // per-instance grass blade world positions
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

out vec3 FragPos;
out vec3 Normal;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec2 TexCoord;

void main()
{
    vec3 instancePosition = grassPositions[gl_InstanceID].xyz;

    vec4 worldPosition = model * vec4(instancePosition + aPos, 1.0);
    gl_Position = projection * view * worldPosition;
    gl_ClipDistance[0] = dot(worldPosition, clippingPlane);

    FragPos = vec3(worldPosition);
    surfaceNormal = normalize((model * vec4(aNormal, 0.0)).xyz);
    Normal = aNormal;
    toLightVector = lightPosition - FragPos;
    TexCoord = aTexCoord;
}