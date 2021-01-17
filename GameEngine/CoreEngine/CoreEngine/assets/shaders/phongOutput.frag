#version 330 core

uniform sampler2D inputPosition;
uniform sampler2D inputNormal;
uniform sampler2D inputDiffuse;
uniform sampler2D inputSpecular;
uniform sampler2D inputAmbient;

uniform vec2 resolution;

uniform vec3 attenuation;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightAmbient;
uniform vec2 spotlightAngles; // { inner, outer }
uniform int spotlightFalloff;
uniform int lightType;
uniform float maxRadius;
uniform float lightBrightness;

uniform bool shadowDebugView;
uniform bool shadowsEnabled;

uniform mat4 cameraTransform;

uniform vec2 shadowScale;

uniform sampler2D shadowLeft;
uniform sampler2D shadowRight;
uniform sampler2D shadowFront;
uniform sampler2D shadowBack;
uniform sampler2D shadowTop;
uniform sampler2D shadowBottom;

//layout (early_fragment_tests) in;

layout(location = 0) out vec4 lighting;
layout(location = 1) out vec4 glowOutputColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution;
	vec3 position = texture(inputPosition, uv).xyz;
	vec3 look = -normalize(position);
	vec3 normal = texture(inputNormal, uv).xyz;
	vec4 diffuseData = texture(inputDiffuse, uv);
	vec3 diffuse = diffuseData.xyz;
	vec3 specular = texture(inputSpecular, uv).xyz;
	vec3 ambient = texture(inputAmbient, uv).xyz;
	int shininess = int(diffuseData.w * 255);
	
	vec3 lightVector = lightPosition - position;
	
	if (lightType == 0)
		lightVector = lightDirection;
	
	float distanceSquared = dot(lightVector, lightVector);
	
	float lightDistance = sqrt(distanceSquared);
	lightVector *= 1 / lightDistance;
	
	float shadowDistance = 1e25;
	vec3 col = vec3(0,0,0);
	float spotlightDot = dot(lightDirection, lightVector);
	
	if (
		(lightType == 2 && spotlightDot < spotlightAngles.y) ||
		(lightType != 0 && maxRadius * 1.1f < lightDistance)
	)
		shadowDistance = 0;
	
	if (shadowsEnabled && shadowDistance != 0)
	{
		vec3 camLightVector = (cameraTransform * vec4(-lightVector, 0)).xyz;
		vec3 absLightVector = abs(camLightVector);
		
		if (absLightVector.x > absLightVector.y && absLightVector.x > absLightVector.z)
		{
			if (camLightVector.x > 0)
				shadowDistance = texture(shadowRight, shadowScale * 0.5f * (1 + vec2(1, 1) * camLightVector.zy / absLightVector.x)).r;
			else
				shadowDistance = texture(shadowLeft, shadowScale * 0.5f * (1 + vec2(-1, 1) * camLightVector.zy / absLightVector.x)).r;
		}
		else if (absLightVector.y > absLightVector.x && absLightVector.y > absLightVector.z)
		{
			if (camLightVector.y > 0)
				shadowDistance = texture(shadowTop, shadowScale * 0.5f * (1 + vec2(1, 1) * camLightVector.xz / absLightVector.y)).r;
			else
				shadowDistance = texture(shadowBottom, shadowScale * 0.5f * (1 + vec2(1, -1) * camLightVector.xz / absLightVector.y)).r;
		}
		else
		{
			if (camLightVector.z > 0)
				shadowDistance = texture(shadowBack, shadowScale * 0.5f * (1 + vec2(-1, 1) * camLightVector.xy / absLightVector.z)).r;
			else
				shadowDistance = texture(shadowFront, shadowScale * 0.5f * (1 + (vec2(1, 1) * camLightVector.xy / absLightVector.z))).r;
		}
		
		col = normalize(camLightVector) * 0.1;
	}
	
	float spotlight = 1;
	
	if (shadowDistance - 0.001 > lightDistance)
	{
		float lightDot = dot(lightVector, normal);
		
		if (!shadowsEnabled || lightDot > 0)
		{
			vec3 reflected = 2 * lightDot * normal - lightVector;
			
			float attenuationFactor = 1;
			
			if (lightType != 0)
			{
				if (lightDistance < maxRadius)
					attenuationFactor = max(min(1 / (attenuation.x + lightDistance * attenuation.y + distanceSquared * attenuation.z), 1), 0);
				else
					attenuationFactor = max(0, (10 - 10 * 100 / (maxRadius * maxRadius) * (lightDistance - maxRadius) * (lightDistance - maxRadius)) / 255);
			}
			
			int falloff = int(lightType == 2) * spotlightFalloff + int(lightType != 2);
			
			if (lightType == 2)
			{
				if (spotlightDot < spotlightAngles.y)
					spotlight = 0;
				else if (spotlightDot < spotlightAngles.x)
					spotlight = pow((spotlightDot - spotlightAngles.y) / (spotlightAngles.x - spotlightAngles.y), falloff);
			}
			
			lighting = vec4(lightBrightness * attenuationFactor * (
				spotlight * (lightAmbient * ambient + 
					lightDiffuse * diffuse * max(lightDot, 0) + lightSpecular * specular * pow(max(dot(reflected, look), 0), shininess)
				)
			), 1);
		}
	}
	
	glowOutputColor = vec4(0, 0, 0, 0);
	//glowOutputColor =  + log2(2 * lighting * lighting);
	
	if (shadowDebugView && !(normal.x == 0 && normal.y == 0 && normal.z == 0) && shadowsEnabled)
		lighting = vec4(
			min(max(shadowDistance - lightDistance, 0), 100) / 200,
			1 - min(abs(shadowDistance - lightDistance), 100) / 200,
			min(max(lightDistance - shadowDistance, 0), 100) / 200,
			1
		);
}