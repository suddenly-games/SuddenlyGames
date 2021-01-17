#version 330 core

uniform sampler2D inputData;
uniform sampler2D luminescenceData;

uniform float exposure;
uniform float burnoutCutoff;

uniform int rangeFittingMode;

uniform vec2 resolution;

uniform int luminescenceMode;

layout(location = 0) out vec4 outputValue;

float luminance(vec4 color)
{
	// values based on relative luminance in RGB
	float value = 0;
	
	if (luminescenceMode == 0)
		value = dot(vec3(0.2126, 0.7152, 0.0722), color.rgb);
	else if (luminescenceMode == 1)
		value = dot(vec3(0.299, 0.587, 0.114), color.rgb);
	else if (luminescenceMode == 2)
		value = sqrt(dot(vec3(0.2126, 0.7152, 0.0722), color.rgb * color.rgb));
	
	return value;
}

vec3 power(vec3 vector, float power)
{
	return vec3(
		pow(vector.x, power),
		pow(vector.y, power),
		pow(vector.z, power)
	);
}

void main()
{
	vec2 luminescenceValues = texture(luminescenceData, vec2(0, 0)).rg;
	vec4 color = texture(inputData, gl_FragCoord.xy / resolution);
	float luminescence = luminescenceValues.r;
	float maxLuminescence = luminescenceValues.g;
	
	if (rangeFittingMode == 0)
	{
		vec3 scaledColor = exposure * luminance(color) / luminescence * color.rgb;
		
		outputValue = vec4(scaledColor / (vec3(1, 1, 1) + scaledColor), 1);
	}
	else if (rangeFittingMode == 1)
	{
		float intensity = luminance(color);
		
		vec3 scaledColor = exposure * intensity / luminescence * (color.rgb + intensity * vec3(1/255, 1/255, 1/255));
		
		float burnoutRange = max(burnoutCutoff, maxLuminescence * 0.9f);
		
		burnoutRange *= burnoutRange;
		
		outputValue = vec4(scaledColor * (1 + scaledColor / burnoutRange) / (1 + scaledColor), 1);
	}
	
	outputValue = vec4(power(outputValue.xyz, 1 / 2.2), outputValue.w);
}