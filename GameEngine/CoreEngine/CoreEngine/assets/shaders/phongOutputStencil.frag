#version 330 core

uniform sampler2D inputPosition;

uniform vec2 resolution;

uniform vec3 attenuation;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec2 spotlightAngles; // { inner, outer }
uniform int lightType;
uniform float maxRadius;

uniform bool shadowsEnabled;

uniform mat4 cameraTransform;

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
	//vec2 uv = gl_FragCoord.xy / resolution;
	//vec3 position = texture(inputPosition, uv).xyz;
	//
	//vec3 lightVector = lightPosition - position;
	//
	//if (lightType != 0)
	//{
	//	float distanceSquared = dot(lightVector, lightVector);
	//	
	//	if (distanceSquared < maxRadius * maxRadius)
	//	{
	//		float shadowDistance = 1e25;
	//		
	//		if (shadowsEnabled)
	//		{
	//			vec3 camLightVector = (cameraTransform * vec4(-lightVector, 0)).xyz;
	//			vec3 absLightVector = abs(camLightVector);
	//			
	//			if (absLightVector.x > absLightVector.y && absLightVector.x > absLightVector.z)
	//			{
	//				if (camLightVector.x > 0)
	//					shadowDistance = texture(shadowLeft, 0.5f * (1 + vec2(1, 1) * camLightVector.zy / absLightVector.x)).r;
	//				else
	//					shadowDistance = texture(shadowRight, 0.5f * (1 + vec2(-1, 1) * camLightVector.zy / absLightVector.x)).r;
	//			}
	//			else if (absLightVector.y > absLightVector.x && absLightVector.y > absLightVector.z)
	//			{
	//				if (camLightVector.y > 0)
	//					shadowDistance = texture(shadowBottom, 0.5f * (1 + vec2(1, 1) * camLightVector.xz / absLightVector.y)).r;
	//				else
	//					shadowDistance = texture(shadowTop, 0.5f * (1 + vec2(1, -1) * camLightVector.xz / absLightVector.y)).r;
	//			}
	//			else
	//			{
	//				if (camLightVector.z > 0)
	//					shadowDistance = texture(shadowBack, 0.5f * (1 + vec2(-1, 1) * camLightVector.xy / absLightVector.z)).r;
	//				else
	//					shadowDistance = texture(shadowFront, 0.5f * (1 + (vec2(1, 1) * camLightVector.xy / absLightVector.z))).r;
	//			}
	//		}
	//		
	//		shadowDistance *= shadowDistance;
	//		
	//		if (shadowDistance + 1e-2 >= distanceSquared)
	//		{
	//			if (lightType == 2)
	//			{
	//				float spotlightDot = dot(lightDirection, lightVector / sqrt(distanceSquared));
	//				
	//				//if (spotlightDot < spotlightAngles.y)
	//				//	discard;//gl_FragDepth = 2; // cut stencil
	//				//else
	//				//{
	//				//	// mark stencil
	//				//}
	//			}
	//			else
	//			{
	//				// mark stencil
	//			}
	//		}
	//		//else
	//			//discard;//gl_FragDepth = 2; // cut stencil
	//	}
	//	//else
	//		//discard;//gl_FragDepth = 2; // cut stencil
	//}
	//else
	//{
	//	// mark stencil
	//}
	
	lighting = vec4(0, 0, 0, 0);
	
	glowOutputColor = vec4(0, 0, 0, 0);
}