#version 330 core

uniform vec2 offset;

layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location = 0) out vec4 outputColor;

float noise(vec2 coord)
{
	float value = sin(12.9898 * coord.x + 78.233 * coord.y) * 43758.5453;
	
	return fract(value);
}

void main()
{
	float value = noise(gl_FragCoord.xy - vec2(0.5, 0.5) + offset) * 2;
	
	outputColor = vec4(value, value, value, 1);
}