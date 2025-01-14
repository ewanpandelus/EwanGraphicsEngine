

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
uniform sampler2D dudvMap;
uniform sampler2D depthTexture;
uniform sampler2D normalMap;
uniform float time;


const float waveStrength = 0.2f;
float waterSpeed = 0.06f;

void main()
{
	vec4 waterColour1 = vec4(0.5, 0.97, 0.98, 1);
	vec4 waterColour2 = vec4(0.06, 0.06,0.2,1);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);
	vec3 norm = normalize(Normal);
	vec3 unitLightVector = normalize(toLightVector);

	waterSpeed *= time;


	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, 1 -ndc.y);


	float near = 0.1; 
    float far  = 2000.0; 
    float depth = texture(depthTexture, refractTexCoords).r;

	
	

	float eye_z = near * far / ((depth * (far - near)) - far);
	float floorDistance = ( eye_z - (-near) ) / ( -far - (-near) );
	

	depth = gl_FragCoord.z;
	eye_z = near * far / ((depth * (far - near)) - far);
	float waterDistance = ( eye_z - (-near) ) / ( -far - (-near) );



	float depthMultiplier = 60;
	float alphaMultiplier = 60;
	float distortionMultplier = 8;

	float waterDepth = (floorDistance - waterDistance);
	float opticalDepth01 = 1 - exp(-waterDepth * depthMultiplier);
	float alpha = 1 - exp(-waterDepth * alphaMultiplier);
	float disortionDepthMultiplier =  1 - exp(-waterDepth * distortionMultplier);
	vec4 waterColour = mix(waterColour1, waterColour2, opticalDepth01);


	vec2 scaledTexCoords = TexCoord * 6;

	vec2 distortedTexCoords = texture(dudvMap, vec2(scaledTexCoords.x + waterSpeed, scaledTexCoords.y)).rg*0.1;
	distortedTexCoords = scaledTexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+waterSpeed);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * disortionDepthMultiplier;

	totalDistortion *= waveStrength;

	refractTexCoords += totalDistortion;
	refractTexCoords  = clamp(refractTexCoords, 0.001,0.999);

	reflectTexCoords += totalDistortion;
    reflectTexCoords  = clamp(reflectTexCoords, 0.001,0.999);


	vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColour = texture(refractionTexture, refractTexCoords);
	vec4 normalMapColour = texture(normalMap, distortedTexCoords);

	vec4 outColour = mix(reflectColour, refractColour, 0.2);
	outColour =  mix(outColour, waterColour, 0.6);
	
	outColour.a = alpha;


	float nDot1 = dot(norm, vec3(0.5, 1, 1));
	float brightness = max(nDot1, 0.0);
	vec4 diffuse = brightness * vec4(0.5,0.5,0.5,0.5);
    FragColor = outColour;
};