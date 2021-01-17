#version 330 core

uniform vec2 resolution;
uniform sampler2D textureData;
uniform sampler2D blurData;

layout(location = 0) out vec4 outputColor;

void main()
{
	vec2 uv = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y);
	vec4 input1 = texture2D(textureData, uv);
	vec4 input2 = texture2D(blurData, uv);
	outputColor = vec4(input1.rgb + input2.rgb, 1);
}