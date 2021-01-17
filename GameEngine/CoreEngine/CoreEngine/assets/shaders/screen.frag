#version 330 core

uniform sampler2D textureData;
uniform sampler2D clippingMask;

uniform vec4 color;
uniform vec4 textureColor;

uniform vec2 uvScale;
uniform vec2 uvOffset;
uniform vec2 resolution;

uniform bool useTexture;
uniform bool blendTexture;
uniform bool useClippingMask;

in VertexData
{
	vec2 position;
	vec2 uv;
} inputData;

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
	vec4 pixelColor = color;
	
	if (useTexture)
	{
		vec4 texturePixelColor = texture(textureData, inputData.uv * uvScale + uvOffset) * textureColor;
		
		if (blendTexture)
			pixelColor = blendColor(texturePixelColor, color);
		else
			pixelColor *= texturePixelColor;
	}
	
	if (useClippingMask)
		pixelColor = pixelColor *= texture(clippingMask, gl_FragCoord.xy / resolution.xy);
	
	outputColor = pixelColor;
}