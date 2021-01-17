#version 330 core

uniform mat4 cameraTransform;

uniform vec4 color;

uniform int wrappingMode;
uniform bool textureEnabled;
uniform bool blendTexture;
uniform bool normalMapEnabled;
uniform bool specularMapEnabled;
uniform bool horizonMapEnabled;
uniform bool skyboxEnabled;
uniform sampler2D textureData;
uniform sampler2D normalMapData;
uniform sampler2D specularMapData;
uniform sampler2D horizonMapData;
uniform sampler2D cubeMapFront;
uniform sampler2D cubeMapBack;
uniform sampler2D cubeMapLeft;
uniform sampler2D cubeMapRight;
uniform sampler2D cubeMapTop;
uniform sampler2D cubeMapBottom;
uniform vec4 textureColor;
uniform vec3 boxScale;
uniform vec3 meshCenter;
uniform bool singleCubeMapped;
uniform bool compressedNormalMap;

uniform vec3 glowColor;

// material
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform vec3 materialAmbient;
uniform vec3 materialEmissive;
uniform int materialShininess;
uniform float refractionRatio;
uniform bool reflective;
uniform float reflectivity;
uniform mat4 objectRotation;

uniform bool flipCubeMapV;

uniform vec2 uvScale;
uniform vec2 uvOffset;

uniform bool useOffsetMap;
uniform float offsetAmplitude;
uniform vec2 offsetMinCoord;
uniform vec2 offsetMaxCoord;
uniform sampler2D offsetMap;

in PhongVertexData
{
	vec2 uv;
	vec3 position;
	vec3 normal;
	vec3 basePos;
	vec3 baseNormal;
	flat vec3 tVector;
	flat vec3 bVector;
} inputData;

//layout (early_fragment_tests) in;

layout(location = 0) out vec4 deferredPosition;
layout(location = 1) out vec4 deferredNormal;
layout(location = 2) out vec4 deferredDiffuse;
layout(location = 3) out vec4 deferredSpecular;
layout(location = 4) out vec4 deferredAmbient;
layout(location = 5) out vec4 deferredEmissive;
layout(location = 6) out vec4 deferredGlow;
//layout(location = 7) out vec4 deferredHorizon;

float blendColorChannel(float src, float srcAlpha, float dst, float dstAlpha, float alpha)
{
	return (src * srcAlpha + dst * dstAlpha * (1 - srcAlpha)) / alpha;
}

vec4 blendColor(vec4 srcColor, vec4 dstColor)
{
	float alpha = srcColor.w + dstColor.w * (1 - srcColor.w);
	
	return vec4(
		blendColorChannel(srcColor.x, srcColor.w, dstColor.x, dstColor.w, alpha),
		blendColorChannel(srcColor.y, srcColor.w, dstColor.y, dstColor.w, alpha),
		blendColorChannel(srcColor.z, srcColor.w, dstColor.z, dstColor.w, alpha),
		alpha
	);
}

void main()
{
	vec4 color = color;
	vec2 uv = inputData.uv;
	vec3 tVector = inputData.tVector;
	vec3 bVector = inputData.bVector;
	
	if (singleCubeMapped)
	{
		float v = flipCubeMapV ? -1 : 1;
		vec3 absBaseNormal = abs(inputData.baseNormal);
		vec3 fragPos = 0.5f * (inputData.basePos + vec3(1, 1, 1)) * boxScale;
		vec3 shifts = floor(fragPos);
		fragPos = 2 * fract(fragPos) - vec3(1, 1, 1);
		vec3 absFragPos = abs(fragPos);
		vec3 normal = inputData.baseNormal;
		float dirX = 1;
		float dirY = v;
		
		if (absBaseNormal.x >= absBaseNormal.y && absBaseNormal.x > absBaseNormal.z)
		{
			int dir = 1 - 2 * int(absBaseNormal.x > 0);
			
			dirX = float(dir);
			uv = 0.5f * (1 + vec2(dir, v) * fragPos.zy) + vec2(dir, v) * shifts.zy;
		}
		else if (absBaseNormal.y > absBaseNormal.x && absBaseNormal.y > absBaseNormal.z)
		{
			int dir = 1 - 2 * int(absBaseNormal.y > 0);
			
			dirY *= float(dir);
			uv = 0.5f * (1 + vec2(1, dir * v) * fragPos.xz) + vec2(1, dir * v) * shifts.xz;
		}
		else
		{
			int dir = 1 - 2 * int(absBaseNormal.z < 0);
			
			dirX = float(dir);
			uv = 0.5f * (1 + vec2(dir, v) * fragPos.xy) + vec2(dir, v) * shifts.xy;
		}
		
		int mapX = int(absBaseNormal.x >= absBaseNormal.y) & int(absBaseNormal.x > absBaseNormal.z);
		int mapY = (1 ^ mapX) & int(absBaseNormal.y > absBaseNormal.z);
		
		tVector = dirX * objectRotation[2 * mapX].xyz;
		bVector = dirY * objectRotation[1 + mapY].xyz;
		
		if (flipCubeMapV)
			bVector *= -1;
	}
	
	vec3 specular = materialSpecular;
	int shininess = materialShininess;
	
	if (textureEnabled)
	{
		
		vec4 pixelColor = texture(textureData, uv);
		
		if (blendTexture)
			color = blendColor(pixelColor * textureColor, color);
		else
			color *= pixelColor * textureColor;
	}
	
	if (specularMapEnabled)
	{
		vec4 pixelColor = texture(specularMapData, uv);
		
		specular *= pixelColor.r;
		shininess = int(pixelColor.g * 255) + 1;
	}
	
	specular = max(specular, vec3(1, 1, 1) / 255);
	
	vec3 look = inputData.position;// - cameraTransform[3].xyz;
	float camDistance = length(look);
	look = -look / camDistance;
	vec3 normal = normalize(inputData.normal);
	
	if (useOffsetMap)
	{
		vec2 uv = (inputData.basePos.xz - offsetMinCoord) / (offsetMaxCoord - offsetMinCoord);
		vec4 offsetData = texture(offsetMap, uv);
		
		// calc normal
		float d = 10.0 / 64; // Hard-coded bad stuff
		normal = (objectRotation * vec4(normalize(cross(vec3(d, offsetData.y, 0), vec3(0, offsetData.z, d))), 0)).xyz;
	}
	else if (normalMapEnabled)
	{
		vec4 mapNormal = texture(normalMapData, uv);
		
		if (compressedNormalMap)
		{
			mapNormal.x = 2 * mapNormal.a - 1;
			mapNormal.y = 2 * mapNormal.g - 1;
			mapNormal.z = sqrt(1 - mapNormal.x * mapNormal.x - mapNormal.y * mapNormal.y);
			mapNormal.w = 0;
		}
		else
			mapNormal = vec4(2 * mapNormal.xyz - vec3(1, 1, 1), 0);
		
		normal = normalize(mat3(tVector, bVector, normal) * mapNormal.xyz);
	}
	
	vec4 horizon = vec4(0, 0, 0, 0);
	
	//if (horizonMapEnabled)
	//{
	//	vec4 horizons = texture(horizonMapData, uv);
	//	
	//	// convert to screen oriented
	//	// project screen space axis onto tangent space horizons
	//	
	//	//if ( 
	//}
	
	deferredPosition = vec4(inputData.position, 1);
	deferredNormal = vec4(normal, 1);
	deferredDiffuse = vec4(color.xyz * materialDiffuse, float(shininess) / 255);
	deferredSpecular = vec4(specular, 1);
	deferredAmbient = vec4(color.xyz * materialAmbient, 1);
	deferredEmissive = vec4(materialEmissive, 1);
	deferredGlow = vec4(glowColor, 1);
	//deferredHorizon = horizon;
}