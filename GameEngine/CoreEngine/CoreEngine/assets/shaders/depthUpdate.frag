#version 330 core

uniform sampler2D inputPosition;
uniform vec2 resolution;
uniform vec2 projectionData;

layout(location = 0) out vec4 lighting;
layout(location = 1) out vec4 glow;

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution;
	vec4 position = texture(inputPosition, uv);
	
	gl_FragDepth = position.w;//-(projectionData.x / position.z + projectionData.y);//(position.z * projectionData.x + projectionData.y) / -position.z;
	
	lighting = vec4(0, 0, 0, 0);
	glow = vec4(0, 0, 0, 0);
}