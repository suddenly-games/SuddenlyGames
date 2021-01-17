#version 330 core

uniform sampler2D backgroundData;
uniform sampler2D textureData;
uniform vec4 color;
uniform vec2 resolution;
uniform vec2 textureSize;
uniform vec2 texturePosition;
uniform bool subtractMode;

layout(location = 0) out vec4 outputColor;

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
	vec4 backgroundColor = texture(backgroundData, vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y));
	outputColor = backgroundColor;
	
	vec2 uv = vec2((gl_FragCoord.x - texturePosition.x) / textureSize.x, (gl_FragCoord.y - texturePosition.y) / textureSize.y);
	
	if (uv.x >= 0 && uv.x <= 1 && uv.y >= 0 && uv.y <= 1)
	{
		vec4 textureColor = texture(textureData, uv) * color;
		
		if (subtractMode)
		{
			outputColor.x -= textureColor.w * textureColor.x;
			outputColor.y -= textureColor.w * textureColor.y;
			outputColor.z -= textureColor.w * textureColor.z;
			outputColor.w -= textureColor.w;
		}
		else
			outputColor = blendColor(textureColor, backgroundColor);
	}
}