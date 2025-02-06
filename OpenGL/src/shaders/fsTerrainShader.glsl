

#version 330 core
out vec4 FragColor;
in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;
in vec4 vertexPos;


uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D normalMap;

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
	vec3 norm = normalize(Normal);
	float slope = 1-norm.y;
	float noiseFreq = 0.01f;

	vec4 sandColour = vec4(0.96,0.95,0.73,1);
	vec4 grassColour = vec4(0.3,0.6,0.4,1);
	vec4 groundColour = vec4(0.4, 0.33,0.34, 1);
	vec4 rockColour = vec4(0.23,0.23,0.23,1);
	vec4 snowColour = vec4(0.96,0.96,0.96,1);


	vec4 mountainColour = mix(groundColour, rockColour, clamp(slope * 5, 0, 1));
	vec4 topMountainColour= mix(snowColour, rockColour, clamp(slope * 5, 0, 1));

	vec4 terrainColour = vec4(0,0,0,0);
	
	float yPosWithNoise = vertexPos.y + snoise(vec2(vertexPos.x *noiseFreq, vertexPos.z *noiseFreq)) * 5;

	if(yPosWithNoise < 0)
	{
		terrainColour = sandColour;
	}
	if(yPosWithNoise>=0 && yPosWithNoise<50)
	{
		terrainColour = mix(sandColour, grassColour, clamp((yPosWithNoise)/50, 0, 1));
	}
	if(yPosWithNoise >=50 && yPosWithNoise <180)
	{
		terrainColour = mix(grassColour, mountainColour, clamp((yPosWithNoise - 50)/130, 0, 1));
	}
	if(yPosWithNoise >=180 && yPosWithNoise < 230)
	{
		terrainColour = mountainColour;
	}

	if(yPosWithNoise >=230)
	{
		terrainColour = mix(mountainColour, topMountainColour, clamp((yPosWithNoise - 230)/50, 0, 1));
	}


	//terrainColour = vec4((yPosWithNoise -5)/10, (yPosWithNoise -5)/10, (yPosWithNoise -5)/10, 1);
	// Normalize the normal

	vec3 normalFromMap = texture(normalMap, TexCoord * 128).rgb;
	normalFromMap = vec3(normalFromMap.r * 2.0 - 1.0, normalFromMap.b, normalFromMap.g * 2.0 - 1.0);
	normalFromMap = normalize(normalFromMap);


    // Blend vertex normal and normal map
    vec3 finalNormal = normalize(mix(Normal, normalFromMap, 0.1));


    // Compute light direction
    vec3 lightDir = normalize(lightPos - FragPos);

    // Compute view direction
//    vec3 viewDir = normalize(viewPos - FragPos);

    // Blinn-Phong Lighting Model

    // Diffuse lighting
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting (Blinn-Phong)
    //vec3 halfwayDir = normalize(lightDir + viewDir); 
   // float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0); // Shininess = 64
 //   vec3 specular = spec * lightColor * 0.5; // Reduce intensity
//
    // Ambient lighting (softens the effect)
    vec3 ambient = lightColor * 0.3;

    // Final color calculation
    vec4 finalColor = vec4((ambient + diffuse),1) * terrainColour;
    
    FragColor = finalColor;
};