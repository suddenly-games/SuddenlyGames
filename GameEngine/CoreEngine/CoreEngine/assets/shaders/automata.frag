#version 330 core

uniform vec2 size;
uniform sampler2D inputTexture;

layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location = 0) out vec4 outputColor;

void main()
{
	float total = 0;
	
	for (int x = -1; x < 2; ++x)
		for (int y = -1; y < 2; ++y)
			if (!(x == 0 && y == 0))
				total += texture(inputTexture, (gl_FragCoord.xy + vec2(x, y)) / size).r;
	
	outputColor = total > 5 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1);
}