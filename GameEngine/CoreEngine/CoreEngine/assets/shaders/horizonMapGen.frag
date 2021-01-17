#version 330 core

uniform sampler2D normalMap;

uniform int scanLength;

uniform vec2 resolution;

uniform bool compressedNormalMap;

in VertexData
{
	vec4 position;
	vec4 normal;
	vec4 color;
	vec4 uv;
	float depth;
} inputData;

layout(location = 0) out vec4 outputHorizon;

int computeSlope(vec2 uv, vec3 normal, vec2 inc, vec2 incH)
{
	float height = 0;
	
	
	float slope = height / float(scanLength) * inc;
}

void main()
{
	int slopeN = 0;
	int slopeS = 0;
	int slopeE = 0;
	int slopeW = 0;
	int slopeNE = 0;
	int slopeNW = 0;
	int slopeSE = 0;
	int slopeSW = 0;
	
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec3 inc = vec3(vec2(1, 1) / resolution.xy, 0);
	vec3 incH = 0.5f * inc;
	
	vec3 normal = texture(normalMap, uv).xyz;
	
	slopeN  = computeSlope(uv, normal,  inc.zy,  incH.zy);
	slopeS  = computeSlope(uv, normal, -inc.zy, -incH.zy);
	slopeE  = computeSlope(uv, normal, -inc.xz, -incH.xz);
	slopeW  = computeSlope(uv, normal,  inc.xz,  incH.xz);
	slopeNE = computeSlope(uv, normal,  inc.zy - inc.xz,  incH.zy - incH.xz);
	slopeNW = computeSlope(uv, normal,  inc.zy + inc.xz,  incH.zy + incH.xz);
	slopeSE = computeSlope(uv, normal, -inc.zy - inc.xz, -incH.zy - incH.xz);
	slopeSW = computeSlope(uv, normal, -inc.zy + inc.xz, -incH.zy + incH.xz);
	
	outputHorizon = vec4(
		float(slopeN * 16 + slopeS) / 255,
		float(slopeE * 16 + slopeW) / 255,
		float(slopeNE * 16 + slopeNW) / 255,
		float(slopeSE * 16 + slopeSW) / 255
	);
}