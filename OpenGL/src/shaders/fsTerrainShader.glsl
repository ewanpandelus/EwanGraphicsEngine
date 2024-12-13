

#version 330 core
out vec4 FragColor;
in vec3 FragPos;  
in vec3 Normal;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec2 TexCoord;


uniform vec4 tryColor; // we set this variable in the OpenGL code.
uniform vec4 lightColour;

uniform sampler2D ourTexture;

void main()
{
	vec4 texColour = texture(ourTexture, TexCoord);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);
	vec3 norm = normalize(Normal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(norm, vec3(0.5, 1, 1));
	float brightness = max(nDot1, 0.0);
	vec4 diffuse = brightness * vec4(0.5,0.5,0.5,1);
	vec4 result = clamp(ambient+diffuse, 0, 1);
	FragColor = result;// *texColour; //vec4(norm, 1);
};