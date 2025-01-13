

#version 330 core
out vec4 FragColor;
in vec3 FragPos;  
in vec3 Normal;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec2 TexCoord;
in vec4 vertexPos;

uniform vec4 tryColor; // we set this variable in the OpenGL code.
uniform vec4 lightColour;

uniform sampler2D ourTexture;

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

void main()
{
	
	vec4 texColour = texture(ourTexture, TexCoord);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);
	vec3 norm = normalize(Normal);
	vec3 unitLightVector = normalize(toLightVector);
	float slope = 1-norm.y;
	float noiseFreq = 0.05f;

	vec4 sandColour = vec4(0.96,0.95,0.73,1);
	vec4 grassColour = vec4(0.3,0.6,0.4,1);
	vec4 groundColour = vec4(0.4, 0.33,0.34, 1);
	vec4 rockColour = vec4(0.23,0.23,0.23,1);
	vec4 snowColour = vec4(0.96,0.96,0.96,1);


	vec4 mountainColour= mix(groundColour, rockColour, clamp(slope*3, 0, 1));
	vec4 topMountainColour= mix(snowColour, rockColour, clamp(slope*3, 0, 1));

	vec4 terrainColour = vec4(0,0,0,0);
	
	float yPosWithNoise = vertexPos.y + snoise(vec2(vertexPos.x *noiseFreq, vertexPos.z *noiseFreq));

	if(yPosWithNoise < 5)
	{
		terrainColour = sandColour;
	}
	if(yPosWithNoise>=5 && yPosWithNoise<30)
	{
		terrainColour = mix(sandColour, grassColour, clamp((yPosWithNoise - 5)/25, 0, 1));
	}
	if(yPosWithNoise >=30 && yPosWithNoise <100)
	{
		terrainColour = mix(grassColour, mountainColour, clamp((yPosWithNoise - 30)/70, 0, 1));
	}
	if(yPosWithNoise >=100)
	{
		terrainColour = mix(mountainColour, topMountainColour, clamp((yPosWithNoise - 100)/20, 0, 1));
	}


	//terrainColour = vec4((yPosWithNoise -5)/10, (yPosWithNoise -5)/10, (yPosWithNoise -5)/10, 1);


	float nDot1 = dot(norm, vec3(0.5, 1, 1));
	float brightness = max(nDot1, 0.0);
	vec4 diffuse = brightness * vec4(0.5,0.5,0.5,1);
	vec4 result = clamp(ambient+diffuse, 0, 1);
	FragColor =  terrainColour* result;// *texColour; //vec4(norm, 1);
};