#version 330 core

/*

This is used for making the matrix texture

*/

// Constants
const float PI = 3.1415926535897932384626433832795;
const int M = 6;
const int N = 6;

// Uniforms
uniform float[M*N] A;     // A matrix
uniform float[M*N] K;     // K matrix
                         
uniform float x_range;   // constant a
uniform float y_range;   // constant b
uniform float c;         // constant c
                         
uniform float time;      // time

uniform vec2 resolution; // resolution of the texture

float getKmn(int m, int n)
{
  return K[m + M * n];
}

float getAmn(int m, int n)
{
  return A[m + M * n];
}


vec4 CalculateU(float x, float y)
{
  float result = 0;
  float Dx = 0;
  float Dy = 0;
  
  float amn, kmn;
  float mPiX, nPiY;
  
  
  for(int m = 0; m < M; ++m)
  {
    for(int n = 0; n < N; ++n)
    {
      mPiX = m * PI * x / x_range;
      nPiY = n * PI * y / y_range;
      
      amn = getAmn(m, n);
      kmn = getKmn(m, n);
      
      result += amn * cos(kmn * c * time) * (sin(mPiX) * sin(nPiY) + cos(mPiX) * cos(nPiY)) / 2;
      Dx     += amn * cos(kmn * c * time) *  (m * PI * sin(nPiY)  * cos(mPiX)  - m * PI * sin(mPiX) * cos(nPiY) ) / 2;
      Dy     += amn * cos(kmn * c * time) *  (n * PI * sin(mPiX)  * cos(nPiY)  - n * PI * sin(nPiY) * cos(mPiX) ) / 2;
    }
  }
  
  return vec4(result, Dx, Dy, 0);
}


layout(location = 0) out vec4 outputValue;

void main()
{
	vec2 pixelCoord = gl_FragCoord.xy;
	
  float x = pixelCoord.x * x_range / 64;;// / resolution.x;
  float y = pixelCoord.y * y_range / 64;;// / resolution.y;
	
	outputValue = CalculateU(x, y);
}