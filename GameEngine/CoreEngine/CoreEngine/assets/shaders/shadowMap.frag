#version 330 core

uniform sampler2D textureData;

uniform vec4 textureColor;

uniform bool textureEnabled;
uniform bool blendTexture;

in ShadowMapVertData
{
	float depth;
	vec2 uv;
} inputData;

//layout (early_fragment_tests) in;

layout(location = 0) out vec4 depth;

float blendColorChannel(float src, float srcAlpha, float dst, float dstAlpha, float alpha)
{
	return (src * srcAlpha + dst * dstAlpha * (1 - srcAlpha)) / alpha;
}

vec4 blendColor(vec4 srcColor, vec4 dstColor)
{
	float alpha = srcColor.w + dstColor.w * (1 - srcColor.w);
	
	return vec4(
		blendColorChannel(srcColor.x, srcColor.w, dstColor.x, dstColor.w, alpha),
		blendColorChannel(srcColor.y, srcColor.w, dstColor.y, dstColor.w, alpha),
		blendColorChannel(srcColor.z, srcColor.w, dstColor.z, dstColor.w, alpha),
		alpha
	);
}

void main()
{
	depth = vec4(inputData.depth, inputData.uv.x, inputData.uv.y, 1);
}