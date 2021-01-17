#version 330 core

in vec3 view;

uniform float skyBrightness;
uniform float skyBackgroundBrightness;
uniform vec4 skyColor;
uniform vec4 skyBackgroundColor;

uniform sampler2D skyLeft;
uniform sampler2D skyRight;
uniform sampler2D skyFront;
uniform sampler2D skyBack;
uniform sampler2D skyTop;
uniform sampler2D skyBottom;

//layout (early_fragment_tests) in;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 glow;

float blendColorChannel(float src, float srcAlpha, float dst, float dstAlpha, float alpha)
{
	return (src * srcAlpha + dst * dstAlpha * (1 - srcAlpha)) / alpha;
}

vec4 blendColor(vec4 srcColor, vec4 dstColor)
{
	float alpha = srcColor.w + dstColor.w * (1 - srcColor.w);
	
	return vec4(
		blendColorChannel(skyBrightness * srcColor.x, srcColor.w, skyBackgroundBrightness * dstColor.x, dstColor.w, alpha),
		blendColorChannel(skyBrightness * srcColor.y, srcColor.w, skyBackgroundBrightness * dstColor.y, dstColor.w, alpha),
		blendColorChannel(skyBrightness * srcColor.z, srcColor.w, skyBackgroundBrightness * dstColor.z, dstColor.w, alpha),
		alpha
	);
}

void main()
{
	vec3 viewVec = normalize(view);
	vec3 absView = abs(viewVec);
	vec4 skyPixel = skyColor;
	
	if (absView.x > absView.y && absView.x > absView.z)
	{
		if (viewVec.x > 0)
			skyPixel = texture(skyLeft, 0.5f * (1 + vec2(-1, 1) * viewVec.zy / absView.x));
		else
			skyPixel = texture(skyRight, 0.5f * (1 + vec2(1, 1) * viewVec.zy / absView.x));
	}
	else if (absView.y > absView.x && absView.y > absView.z)
	{
		if (viewVec.y > 0)
			skyPixel = texture(skyBottom, 0.5f * (1 + vec2(-1, -1) * viewVec.zx / absView.y));
		else
			skyPixel = texture(skyTop, 0.5f * (1 + vec2(-1, 1) * viewVec.zx / absView.y));
	}
	else
	{
		if (viewVec.z > 0)
			skyPixel = texture(skyBack, 0.5f * (1 + vec2(1, 1) * viewVec.xy / absView.z));
		else
			skyPixel = texture(skyFront, 0.5f * (1 + (vec2(-1, 1) * viewVec.xy / absView.z)));
	}
	
	color = vec4(skyBrightness * skyColor.a * skyPixel.a * skyPixel.rgb * skyColor.rgb + skyBackgroundBrightness * skyBackgroundColor.rgb * (skyPixel.a * skyPixel.rgb + (1 - skyPixel.a) * vec3(1, 1, 1)), 1);
	//color = blendColor(skyPixel * skyColor, skyBackgroundColor);
	glow = vec4(0, 0, 0, 0);
}