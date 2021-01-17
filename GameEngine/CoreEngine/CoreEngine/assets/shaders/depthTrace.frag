#version 330 core

layout(location = 0) out vec4 depthOutput;

//layout (early_fragment_tests) in;

in vec3 position;

void main()
{
	depthOutput = vec4(length(position), 0, 0, 0);
}