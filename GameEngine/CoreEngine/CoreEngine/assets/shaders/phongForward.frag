#version 330 core

uniform mat4 cameraTransform;

uniform vec4 color;

uniform int wrappingMode;
uniform bool textureEnabled;
uniform bool blendTexture;
uniform bool normalMapEnabled;
uniform bool specularMapEnabled;
uniform sampler2D textureData;
uniform sampler2D normalMapData;
uniform sampler2D specularMapData;
uniform vec4 textureColor;
uniform vec4 glowColor;
uniform vec3 specularColor;
uniform vec3 boxScale;
uniform vec3 meshCenter;
uniform bool singleCubeMapped;
uniform bool compressedNormalMap;

// material
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform vec3 materialAmbient;
uniform vec3 materialEmissive;
uniform int materialShininess;
uniform mat4 objectRotation;

// global lighting
uniform vec3 globalLightDirection;
uniform vec3 globalDiffuse;
uniform vec3 globalSpecular;
uniform vec3 globalAmbient;
uniform vec3 globalEmissive;
uniform float globalBrightness;

uniform bool flipCubeMapV;

// light object(s)
uniform vec3 attenuation[50];
uniform vec3 lightPosition[50];
uniform vec3 lightDirection[50];
uniform vec3 lightDiffuse[50];
uniform vec3 lightSpecular[50];
uniform vec3 lightAmbient[50];
uniform vec2 spotlightAngles[50]; // { inner, outer }
uniform int spotlightFalloff[50];
uniform int lightType[50];
uniform int activeLights;

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

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 glowOutputColor;

mat3 lightIntensity(int i, vec3 look, vec3 normal, vec3 fragPosition, vec3 specular, int shininess)
{
	vec3 lightVector = lightPosition[i] - fragPosition;
	
	if (lightType[i] == 0)
		lightVector = lightDirection[i];
	
	float distanceSquared = dot(lightVector, lightVector);
	float lightDistance = sqrt(distanceSquared);
	lightVector *= 1 / lightDistance;
	float lightDot = dot(lightVector, normal);
	vec3 reflected = 2 * lightDot * normal - lightVector;
	
	float attenuationFactor = min(1 / (attenuation[i].x + lightDistance * attenuation[i].y + distanceSquared * attenuation[i].z), 1);
	float spotlight = 1;
	
	if (lightType[i] == 2)
	{
		float spotlightDot = dot(lightDirection[i], lightVector);
		
		if (spotlightDot < spotlightAngles[i].y)
			spotlight = 0;
		else if (spotlightDot < spotlightAngles[i].x)
			spotlight = pow((spotlightDot - spotlightAngles[i].y) / (spotlightAngles[i].x - spotlightAngles[i].y), spotlightFalloff[i]);
	}
	
	return spotlight * attenuationFactor * mat3(
		lightDiffuse[i] * materialDiffuse.rgb * max(lightDot, 0),
		lightSpecular[i] * specular * pow(max(dot(reflected, look), 0), shininess),
		lightAmbient[i] * materialAmbient.rgb
	);
	
	//return attenuationFactor * (
	//	lightAmbient[i] * materialAmbient.rgb + spotlight * (
	//		lightDiffuse[i] * materialDiffuse.rgb * max(lightDot, 0) + lightSpecular[i] * specular * pow(max(dot(reflected, look), 0), shininess)
	//	)
	//);
}

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

float getValue(vec3 color)
{
	return max(max(color.r, color.g), color.b);
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
	
	vec3 specular = materialSpecular.rgb;
	int shininess = materialShininess;
	
	if (textureEnabled)
	{
		
		vec4 pixelColor = texture(textureData, uv);
		
		if (blendTexture)
			color = blendColor(pixelColor * textureColor, color);
		else
			color *= pixelColor* textureColor;
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
		normal = (objectRotation * vec4(
			//normalize(cross(vec3(d, offsetData.y, 0), vec3(0, offsetData.z, d)))
			normalize(cross(vec3(0, offsetData.z, 1),vec3(1, offsetData.y, 0)))
		, 0)).xyz;
	}
	else if (normalMapEnabled)
	{
		vec4 mapNormal = texture(normalMapData, uv);
		
		if (compressedNormalMap)
		{
			mapNormal.x = 2 * mapNormal.z - 1;
			mapNormal.y = 2 * mapNormal.y - 1;
			mapNormal.z = sqrt(1 - (mapNormal.x * mapNormal.x + mapNormal.y * mapNormal.y));
		}
		else
			mapNormal = vec4(2 * mapNormal.xyz - vec3(1, 1, 1), 0);
		
		normal = normalize(mat3(inputData.tVector, inputData.bVector, normal) * mapNormal.xyz);
	}
	
	float lightDot = dot(normal, globalLightDirection);
	vec3 reflected = 2 * lightDot * normal - globalLightDirection;
	
	mat3 accumulation = mat3(
		color.rgb * globalDiffuse * materialDiffuse.rgb * max(lightDot, 0),
		globalSpecular * specular * pow(max(dot(reflected, look), 0), shininess),
		globalAmbient * materialAmbient.rgb * color.rgb
	);
	
	for (int i = 0; i < activeLights; ++i)
		accumulation += lightIntensity(i, look, normal, inputData.position, specular, shininess);//fragLook);
	
	vec3 alphaChannels = vec3(
		color.a + (1 - color.a) * min(getValue(accumulation[0]) * color.a, 1),
		color.a + (1 - color.a) * min(getValue(accumulation[1]) * color.a, 1),
		color.a + (1 - color.a) * min(getValue(accumulation[2]) * color.a, 1)
	);
	
	float maxAlpha = getValue(alphaChannels);
	
	vec3 intensity = globalBrightness * (alphaChannels.r * accumulation[0] + alphaChannels.g * accumulation[1] + alphaChannels.b * accumulation[2]);
	
	if (maxAlpha != 0)
		intensity /= maxAlpha;
	
	vec3 localIntensity = glowColor.rgb + globalEmissive + materialEmissive * color.rgb + intensity;
	
	outputColor = vec4(localIntensity, maxAlpha);
	glowOutputColor = glowColor;
	
	//if (useOffsetMap)
	//{
	//	vec2 uv = (inputData.basePos.xz - offsetMinCoord) / (offsetMaxCoord - offsetMinCoord);
	//	vec4 offsetData = texture(offsetMap, uv);
	//	
	//	// calc normal
	//	float d = 1;//10.0 / 64; // Hard-coded bad stuff
	//	outputColor = vec4(vec3(0.5, 0, 0) * (normalize(cross(vec3(0, offsetData.z, 1),vec3(1, offsetData.y, 0))) + vec3(1, 1, 1)), 1);
	//	//outputColor = texture(offsetMap, uv);
	//	//outputColor = vec4(normal, 1);
	//}
}