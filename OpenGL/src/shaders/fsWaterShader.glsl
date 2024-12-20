

#version 330 core
out vec4 FragColor;
in vec3 FragPos;  
in vec3 Normal;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec2 TexCoord;
in vec4 clipSpace;

uniform vec4 tryColor; // we set this variable in the OpenGL code.
uniform vec4 lightColour;


uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{
	vec4 colour = vec4(0.2, 0.2, 0.8, 1);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);
	vec3 norm = normalize(Normal);
	vec3 unitLightVector = normalize(toLightVector);

	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, 1 -ndc.y);



	vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColour = texture(refractionTexture, refractTexCoords);

	vec4 outColour = mix(reflectColour, refractColour, 0.5);

	float nDot1 = dot(norm, vec3(0.5, 1, 1));
	float brightness = max(nDot1, 0.0);
	vec4 diffuse = brightness * vec4(0.5,0.5,0.5,0.5);

    FragColor = mix(outColour, colour, 0.2);

};