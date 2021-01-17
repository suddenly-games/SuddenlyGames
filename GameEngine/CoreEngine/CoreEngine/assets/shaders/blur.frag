#version 330 core

uniform vec2 resolution;
uniform vec2 blurDirection;
uniform sampler2D textureData;
uniform int blurRadius;
uniform float offsets[50];// = float[](0,0,0);
uniform float weights[50];// = float[](0,0,0);

layout(location = 0) out vec4 outputColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / (resolution - vec2(1, 1));
	vec4 originalColor = texture2D(textureData, uv);
	vec4 color = vec4(originalColor.rgb * originalColor.a, originalColor.a) * weights[0];
	
	float weight = weights[0];
	
	for (int i = 1; i < blurRadius; ++i)
	{
		vec2 direction = offsets[i] * blurDirection / (resolution - vec2(1, 1));
		vec4 blurColor = texture2D(textureData, uv - direction);
		
		color += weights[i] * vec4(blurColor.rgb * blurColor.a, blurColor.a);
		weight += weights[i];
		
		blurColor = texture2D(textureData, uv + direction);
		
		color += weights[i] * vec4(blurColor.rgb * blurColor.a, blurColor.a);
		weight += weights[i];
	}
	
	outputColor = color / weight;
}