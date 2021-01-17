#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec3 vertexTVector;
in vec3 vertexBVector;

uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 cameraFront;
uniform vec2 nearPlaneSize;
uniform float nearPlane;

out vec3 view;

void main()
{
	view = vertexPosition.x * cameraRight + vertexPosition.y * cameraUp + cameraFront;
	
	gl_Position = vec4(vertexPosition.xy, 0.9999999f, 1);
}