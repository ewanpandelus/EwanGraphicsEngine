


#version 330 core

// Noise Funcs from https://github.com/stegu/webgl-noise/blob/master/src/noise2D.glsl 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+10.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


out vec4 FragColor;
in vec3 FragPos;  
in vec3 Normal;
in vec3 fromLightVector;
in vec3 toCameraVector;
in vec2 TexCoord;
in vec4 clipSpace;


uniform vec3 lightColour;
uniform float time;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D depthTexture;
uniform sampler2D normalMap;



const float waveStrength = 0.15f;
float waterSpeed = 0.07f;

float normalMapSiftSpeed = 0.07f;

const float normalMapSiftStrength = 0.5f;


const float shineDamper = 50.0f;
const float reflectivity = 1.f;
const float normalMapBlendNoiseFreq = 0.1f;

void main()
{
	vec4 waterColour1 = vec4(0.5, 0.97, 0.98, 1);
	vec4 waterColour2 = vec4(0.06, 0.06,0.2,1);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

	waterSpeed *= time;
	normalMapSiftSpeed *= time;

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



	float depthMultiplier = 100;
	float alphaMultiplier = 100;
	float distortionMultplier = 8;

	float waterDepth = (floorDistance - waterDistance);
	float opticalDepth01 = 1 - exp(-waterDepth * depthMultiplier);
	float alpha = 1 - exp(-waterDepth * alphaMultiplier);
	float disortionDepthMultiplier =  1 - exp(-waterDepth * distortionMultplier);
	vec4 waterColour = mix(waterColour1, waterColour2, opticalDepth01);


	vec2 scaledTexCoords = TexCoord * 50;
	vec2 distortedTexCoords = texture(dudvMap, vec2(scaledTexCoords.x + normalMapSiftSpeed, scaledTexCoords.y)).rg*0.1;
	distortedTexCoords = scaledTexCoords + (vec2(distortedTexCoords.x, distortedTexCoords.y+normalMapSiftSpeed) * normalMapSiftStrength);
	vec4 normalMapColour = texture(normalMap, distortedTexCoords);

	distortedTexCoords = texture(dudvMap, vec2(scaledTexCoords.x + waterSpeed, scaledTexCoords.y)).rg*0.1;
	distortedTexCoords = scaledTexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+waterSpeed);
	
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * disortionDepthMultiplier;

	totalDistortion *= waveStrength;

	refractTexCoords += totalDistortion;
	refractTexCoords  = clamp(refractTexCoords, 0.001,0.999);

	reflectTexCoords += totalDistortion;
    reflectTexCoords  = clamp(reflectTexCoords, 0.001,0.999);


	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
	refractiveFactor = pow(refractiveFactor, 2);

	vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColour = texture(refractionTexture, refractTexCoords);


	vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b, normalMapColour.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColour * specular * reflectivity * disortionDepthMultiplier;


	vec4 outColour = mix(reflectColour, refractColour, refractiveFactor);
	outColour =  mix(outColour, waterColour, 0.6);
	

	outColour.a = alpha;
    FragColor = outColour + vec4(specularHighlights, 0);
};