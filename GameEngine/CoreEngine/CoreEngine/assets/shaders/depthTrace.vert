#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec3 vertexTVector;
in vec3 vertexBVector;

uniform mat4 transform;
uniform mat4 objectTransform;

out vec3 position;

void main()
{
	gl_Position = transform * vec4(vertexPosition, 1);
	
	position = (objectTransform * vec4(vertexPosition,1)).xyz;
}