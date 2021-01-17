#version 330 core

uniform sampler2D textureData;

uniform bool hasParent;

uniform vec2 resolution;

layout(location = 0) out vec4 outputColor;

void main()
{
	if (hasParent)
		outputColor = texture(textureData, vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y));
	else
		outputColor = vec4(1, 1, 1, 1);
}