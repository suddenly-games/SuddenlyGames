#version 330 core

uniform mat4 transform;

uniform vec4 color;
uniform vec4 objectZTransform;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec3 vertexTVector;
in vec3 vertexBVector;

out ShadowMapVertData
{
	float depth;
	vec2 uv;
} outputData;


void main()
{
	gl_Position = transform * vec4(vertexPosition, 1);
	
	outputData.depth = dot(objectZTransform, vec4(vertexPosition, 1));
}