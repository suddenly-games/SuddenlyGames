#version 330 core

uniform sampler2D heightMap;

uniform float scale;

uniform vec2 resolution;

layout(location = 0) out vec4 outputNormal;

void main()
{
	float nx = texture(heightMap, (vec2(-1,  0) + gl_FragCoord.xy) / resolution.xy).r;
	float ny = texture(heightMap, (vec2( 0, -1) + gl_FragCoord.xy) / resolution.xy).r;
	float px = texture(heightMap, (vec2( 1,  0) + gl_FragCoord.xy) / resolution.xy).r;
	float py = texture(heightMap, (vec2( 0,  1) + gl_FragCoord.xy) / resolution.xy).r;
	
	vec3 s = vec3(1, 0, scale * (nx - px));
	vec3 t = vec3(0, 1, scale * (ny - py));
	
	outputNormal = vec4(0.5f * normalize(cross(s, t)) + vec3(0.5f, 0.5f, 0.5f), 1);
}