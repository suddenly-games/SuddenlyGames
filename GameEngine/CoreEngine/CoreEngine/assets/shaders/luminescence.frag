#version 330 core

uniform sampler2D inputData;

uniform vec2 resolution;
uniform vec2 bounds;

uniform bool firstPass;
uniform bool finalPass;

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
	
	return log(0.01 + value);
}

vec4 result(float value1, float value2, float value3, float value4)
{
	return vec4(
		value1 + value2 + value3 + value4,
		max(max(max(value1, value2), value3), value4),
		0,
		1
	);
}

void main()
{
	vec4 input1 = vec4(0, 0, 0, 0);
	vec4 input2 = vec4(0, 0, 0, 0);
	vec4 input3 = vec4(0, 0, 0, 0);
	vec4 input4 = vec4(0, 0, 0, 0);
	
	vec2 lower = 2 * gl_FragCoord.xy;
	vec2 upper = lower + vec2(1, 1);
	
	if (lower.x < bounds.x && lower.y < bounds.y)
		input1 = texture(inputData, lower / resolution);
	
	if (upper.x < bounds.x && lower.y < bounds.y)
		input2 = texture(inputData, vec2(upper.x, lower.y) / resolution);
	
	if (lower.x < bounds.x && upper.y < bounds.y)
		input3 = texture(inputData, vec2(lower.x, upper.y) / resolution);
	
	if (upper.x < bounds.x && upper.y < bounds.y)
		input4 = texture(inputData, upper / resolution);
	
	if (firstPass)
		outputValue = result(luminance(input1), luminance(input2), luminance(input3), luminance(input4));
	else
		outputValue = result(input1.r, input2.r, input3.r, input4.r);
	
	if (finalPass)
		outputValue = vec4(exp(outputValue.r / resolution.x / resolution.y), outputValue.g, 0, 1);
}