#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

out vec3 Normal;
out vec3 toLightVector;
out vec3 fromLightVector;
out vec3 toCameraVector;
out vec2 TexCoord;
out vec4 clipSpace;
out vec3 worldPos;

out vec3 FragPos;  
void main()
{
   vec4 worldPosition = model * vec4(aPos, 1.0);
   clipSpace = projection * view * model * vec4(aPos, 1);
   gl_Position = clipSpace;
   FragPos = vec3(model * vec4(aPos, 1.0));
   toLightVector = lightPosition - worldPosition.xyz;
   fromLightVector = worldPosition.xyz - lightPosition;
   toCameraVector = cameraPosition - worldPosition.xyz;
   worldPos = worldPosition.xyz;
   Normal = aNormal;
   TexCoord = aTexCoord;
};