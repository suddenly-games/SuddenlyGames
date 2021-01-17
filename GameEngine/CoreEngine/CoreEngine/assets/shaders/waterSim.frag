#version 330 core

/*

This is used for... uhh, something

*/

uniform vec2 resolution;

uniform sampler2D inputData;

uniform bool firstPass;

vec2 getUVCoords(vec2 pixelCoord)
{
	return pixelCoord / resolution;
}

vec4 getPixel(vec2 pixelCoord)
{
	return texture(inputData, getUVCoords(pixelCoord));
}

layout(location = 0) out vec4 outputValue;

/*
quick side notes:
multiple output buffers like outputValue could be drawn to simultaneously if needed, just need to add more to the FrameBuffer

try not to use 'discard' as it de-optimizes the graphics pipeline

the graphics pipeline will optimize out branch paths that use booleans at run time
*/

void main()
{
	vec2 pixelCoord = gl_FragCoord.xy;
	
	if (firstPass)
	{
		// initialization code
	}
	else
	{
		// update code
	}
	
	outputValue = vec4(0, 0, 0, 1);
}