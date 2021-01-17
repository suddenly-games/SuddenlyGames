#version 330 core

uniform float sigma;
uniform float maskWeight;
uniform float circleMaskWeight;

uniform vec2 resolution;
uniform vec2 distortion;
uniform vec2 circleMaskOffset;

uniform vec2 gradient1Size;
uniform vec2 gradient2Size;
uniform vec2 gradient3Size;
uniform vec2 gradient1Scale;
uniform vec2 gradient2Scale;
uniform vec2 gradient3Scale;
uniform vec2 gradient1Offset;
uniform vec2 gradient2Offset;
uniform vec2 gradient3Offset;

uniform vec4 nebulaColor;

uniform sampler2D gradient1Data;
uniform sampler2D gradient2Data;
uniform sampler2D gradient3Data;
uniform sampler2D maskData;

layout(location = 0) out vec4 outputColor;

float perlinDot(sampler2D gradient, vec2 position, vec2 gridPos, vec2 size)
{
	vec4 color = 2 * texture(gradient, gridPos / (size/* + vec2(1, 1)*/)) - vec4(1, 1, 0, 0);
	
	return (position.x - gridPos.x) * color.r + (position.y - gridPos.y) * color.g;
}

float perlin(sampler2D gradient, vec2 position, vec2 size)
{
	vec2 gridPos = floor(position);
	
	vec2 st = vec2(perlinDot(gradient, position, gridPos, size), perlinDot(gradient, position, gridPos + vec2(1, 0), size));
	vec2 uv = vec2(perlinDot(gradient, position, gridPos + vec2(0, 1), size), perlinDot(gradient, position, gridPos + vec2(1, 1), size));
	
	vec2 fraction = fract(position);
	vec2 squared = fraction * fraction;
	
	float S = 3 * squared.x - 2 * squared.x * fraction.x;
	
	float a = st.x + S * (st.y - st.x);
	float b = uv.x + S * (uv.y - uv.x);
	
	return a + (3 * squared.y - 2 * squared.y * fraction.y) * (b - a);
}

float wrapFloat(float x, float minimum, float maximum)
{
	if (x < minimum)
		return x + maximum - minimum;
	else if (x > maximum)
		return x + minimum - maximum;
	else
		return x;
}

vec2 wrap(vec2 point, vec2 minimum, vec2 maximum)
{
	return vec2(wrapFloat(point.x, minimum.x, maximum.x), wrapFloat(point.y, minimum.y, maximum.y));
}

float gauss(float sigma, float x)
{
	return exp(-(x*x) / (2.0f * sigma*sigma));
}

float gauss2(float sigma, float a, float b)
{
	return (1.0f / 6) * (gauss(sigma, a) + 4 * gauss(sigma, 0.5f * (a + b)) + gauss(sigma, b));
}

void main()
{
	float color = clamp(0.5f + perlin(gradient1Data, (gradient1Scale / resolution) * (gl_FragCoord.xy + gradient1Offset), gradient1Size), 0, 1);
	float color2 = clamp(0.5f + perlin(gradient2Data, (gradient2Scale / resolution) * (gl_FragCoord.xy + gradient2Offset), gradient2Size), 0, 1);
	
	vec2 warped = wrap((gradient3Scale / resolution) * (gl_FragCoord.xy + gradient3Offset + distortion * vec2(color, color2)), vec2(0, 0), gradient3Size);
	
	vec2 gaussianPosition = gl_FragCoord.xy - circleMaskOffset * resolution;
	
	float weight = circleMaskWeight * gauss2(sigma, gaussianPosition.x + 0.5f, gaussianPosition.x - 0.5f) * gauss2(sigma, gaussianPosition.y + 0.5f, gaussianPosition.y - 0.5f);
	
	if (weight > 1) weight = 1;
	
	float value = (perlin(gradient3Data, warped, gradient3Size) + 0.5);
	
	outputColor = vec4(nebulaColor.rgb, (1 - value * value) * nebulaColor.a * weight * (/*(1 - maskWeight) * vec3(1, 1, 1) + maskWeight */ texture(maskData, gl_FragCoord.xy / resolution).r));
}