#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec3 vertexTVector;
in vec3 vertexBVector;

uniform mat4 transform;
uniform mat4 objectTransform;
//uniform mat4 cameraTransform;

// material
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform vec3 materialAmbient;
uniform vec3 materialEmmissive;
uniform int materialShininess;

// light object(s)
//uniform vec3 attenuation[50];
//uniform vec3 lightPosition[50];
//uniform vec3 spotlightDirection[50];
//uniform vec3 lightAttributes[50]; // { diffuse, specular, ambient }
//uniform vec2 spotlightAngles[50]; // { inner, outer }
//uniform int spotlightFalloff[50];
//uniform int activeLights;

uniform vec2 uvScale;
uniform vec2 uvOffset;

uniform bool useOffsetMap;
uniform float offsetAmplitude;
uniform vec2 offsetMinCoord;
uniform vec2 offsetMaxCoord;
uniform sampler2D offsetMap;

out PhongVertexData
{
	vec2 uv;
	vec3 position;
	vec3 normal;
	vec3 basePos;
	vec3 baseNormal;
	flat vec3 tVector;
	flat vec3 bVector;
} outputData;

void main()
{
	gl_Position = transform * vec4(vertexPosition, 1);
	
	mat4 vectorTransform = transpose(inverse(objectTransform));
	
	outputData.uv = uvScale * vertexUV + uvOffset;
	outputData.basePos = vertexPosition.xyz;
	outputData.position = (objectTransform * vec4(vertexPosition, 1)).xyz;
	outputData.baseNormal = vertexNormal.xyz;
	outputData.normal = normalize((vectorTransform * vec4(vertexNormal, 0)).xyz);
	outputData.tVector = normalize((vectorTransform * vec4(vertexTVector, 0)).xyz);
	outputData.bVector = normalize((vectorTransform * vec4(vertexBVector, 0)).xyz);
	//outputData.camPosition = cameraTransform[3].xyz;
	
	if (useOffsetMap)
	{
		vec2 uv = (vertexPosition.xz - offsetMinCoord) / (offsetMaxCoord - offsetMinCoord);
		vec4 offsetData = texture(offsetMap, uv);
		
		//outputData.color = vec4(offsetData.x, uv.x, uv.y, 1);
		
		gl_Position = transform * vec4(vertexPosition + vec3(0, offsetAmplitude * offsetData.x, 0), 1);
	}
}