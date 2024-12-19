#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;
uniform vec3 lightPosition;
uniform vec4 plane;
out vec3 ourColor;
out vec4 vertexPos;
out vec3 surfaceNormal;
out vec3 Normal;
out vec3 toLightVector;
out vec2 TexCoord;

out vec3 FragPos;  
void main()
{
   vec4 worldPosition = model * vec4(aPos, 1.0);
   vertexPos = worldPosition;
   gl_ClipDistance[0] = dot(worldPosition, plane);
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   ourColor = vec3(1,1,1); // set ourColor to the input color we got from the vertex data
   surfaceNormal = (model*vec4(aNormal, 0.0)).xyz;
   toLightVector = plane.xyz;// - worldPosition.xyz;
   Normal = aNormal;
   TexCoord = aTexCoord;
};