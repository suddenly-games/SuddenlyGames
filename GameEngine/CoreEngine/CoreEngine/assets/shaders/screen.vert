#version 330 core

uniform mat4 transform;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

out VertexData
{
	vec2 position;
	vec2 uv;
} outputData;

void main()
{
	gl_Position = transform * vec4(vertexPosition, 1);
	
	outputData.position = gl_Position.xy;
	outputData.uv = vertexUV;
}