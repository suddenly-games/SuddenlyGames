#pragma once

#include "Uniform.h"
#include "ShaderProgram.h"

namespace GraphicsEngine
{
	class Texture;
	class Material;
	class FrameBuffer;
}

class WaterMatrixProgram : public ShaderProgram
{
private:
	InitializeProgram("WaterMatrixProgram");
	AttachShader(screenVert);
	AttachShader(waterMatrixFrag);
	LinkProgram();

public:
  Uniform(GLSL::FloatArray, A);
  Uniform(GLSL::FloatArray, K);

  Uniform(GLSL::Float, x_range);
  Uniform(GLSL::Float, y_range);
  Uniform(GLSL::Float, c);

  Uniform(GLSL::Float, time);

  Uniform(GLSL::Vec2, resolution);

  Uniform(GLSL::Matrix, transform);

  float mTime = 0;
private:
	std::weak_ptr<GraphicsEngine::Material> CurrentMaterial;
};

class WaterSimProgram : public ShaderProgram
{
private:
	InitializeProgram("WaterSimProgram");
	AttachShader(screenVert);
	AttachShader(waterSimFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Bool, firstPass);

	Uniform(GLSL::Sampler2D, inputData);

private:
	std::weak_ptr<GraphicsEngine::Material> CurrentMaterial;
};

class PhongProgram : public ShaderProgram
{
private:
	InitializeProgram("PhongProgram");
	AttachShader(phongVert);
	AttachShader(phongFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);
	Uniform(GLSL::Matrix, objectTransform);
	Uniform(GLSL::Matrix, objectRotation);
	Uniform(GLSL::Matrix, cameraTransform);

	Uniform(GLSL::Vec4, color);
	Uniform(GLSL::Vec3, glowColor);

	Uniform(GLSL::Bool, compressedNormalMap);
	Uniform(GLSL::Bool, textureEnabled);
	Uniform(GLSL::Bool, normalMapEnabled);
	Uniform(GLSL::Bool, specularMapEnabled);
	Uniform(GLSL::Bool, blendTexture);
	Uniform(GLSL::Bool, skyboxEnabled);
	Uniform(GLSL::Sampler2D, textureData);
	Uniform(GLSL::Sampler2D, normalMapData);
	Uniform(GLSL::Sampler2D, specularMapData);
	Uniform(GLSL::Sampler2D, cubeMapFront);
	Uniform(GLSL::Sampler2D, cubeMapBack);
	Uniform(GLSL::Sampler2D, cubeMapLeft);
	Uniform(GLSL::Sampler2D, cubeMapRight);
	Uniform(GLSL::Sampler2D, cubeMapTop);
	Uniform(GLSL::Sampler2D, cubeMapBottom);
	Uniform(GLSL::Vec4, textureColor);

	Uniform(GLSL::Bool, singleCubeMapped);
	Uniform(GLSL::Bool, flipCubeMapV);
	Uniform(GLSL::Vec3, boxScale);
	Uniform(GLSL::Vec2, uvScale);
	Uniform(GLSL::Vec2, uvOffset);

	// material
	Uniform(GLSL::Vec3, materialDiffuse);
	Uniform(GLSL::Vec3, materialSpecular);
	Uniform(GLSL::Vec3, materialAmbient);
	Uniform(GLSL::Vec3, materialEmissive);
	Uniform(GLSL::Int, materialShininess);
	Uniform(GLSL::Float, refractionRatio);
	Uniform(GLSL::Float, reflectivity);
	Uniform(GLSL::Bool, reflective);

	Uniform(GLSL::Bool, useOffsetMap);
	Uniform(GLSL::Float, offsetAmplitude);
	Uniform(GLSL::Vec2, offsetMinCoord);
	Uniform(GLSL::Vec2, offsetMaxCoord);
	Uniform(GLSL::Sampler2D, offsetMap);

	void SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);
	void SetNormalMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 1);
	void SetSpecularMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 2);
	void SetMaterial(const std::shared_ptr<GraphicsEngine::Material>& material);

private:
	std::weak_ptr<GraphicsEngine::Material> CurrentMaterial;
};

class PhongOutputProgram : public ShaderProgram
{
private:
	InitializeProgram("PhongOutputProgram");
	AttachShader(screenVert);
	AttachShader(phongOutputFrag);
	LinkProgram();
public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Sampler2D, inputPosition);
	Uniform(GLSL::Sampler2D, inputNormal);
	Uniform(GLSL::Sampler2D, inputDiffuse);
	Uniform(GLSL::Sampler2D, inputSpecular);
	Uniform(GLSL::Sampler2D, inputAmbient);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Float, maxRadius);
	Uniform(GLSL::Float, lightBrightness);
	Uniform(GLSL::Vec3, attenuation);
	Uniform(GLSL::Vec3, lightPosition);
	Uniform(GLSL::Vec3, lightDirection);
	Uniform(GLSL::Vec3, lightDiffuse);
	Uniform(GLSL::Vec3, lightSpecular);
	Uniform(GLSL::Vec3, lightAmbient);
	Uniform(GLSL::Vec2, spotlightAngles); // { inner, outer }
	Uniform(GLSL::Int, spotlightFalloff);
	Uniform(GLSL::Int, lightType);

	Uniform(GLSL::Bool, shadowsEnabled);
	Uniform(GLSL::Bool, shadowDebugView);
	Uniform(GLSL::Vec2, shadowScale);

	Uniform(GLSL::Matrix, cameraTransform);

	Uniform(GLSL::Sampler2D, shadowLeft);
	Uniform(GLSL::Sampler2D, shadowRight);
	Uniform(GLSL::Sampler2D, shadowFront);
	Uniform(GLSL::Sampler2D, shadowBack);
	Uniform(GLSL::Sampler2D, shadowTop);
	Uniform(GLSL::Sampler2D, shadowBottom);

	void SetInputBuffer(const std::shared_ptr<GraphicsEngine::FrameBuffer>& buffer = nullptr);
};

class PhongOutputStencilProgram : public ShaderProgram
{
private:
	InitializeProgram("PhongOutputStencilProgram");
	AttachShader(screenVert);
	AttachShader(phongOutputStencilFrag);
	LinkProgram();
public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Sampler2D, inputPosition);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Float, maxRadius);
	Uniform(GLSL::Vec3, lightPosition);
	Uniform(GLSL::Vec3, lightDirection);
	Uniform(GLSL::Vec2, spotlightAngles); // { inner, outer }
	Uniform(GLSL::Int, lightType);

	Uniform(GLSL::Bool, shadowsEnabled);

	Uniform(GLSL::Sampler2D, shadowLeft);
	Uniform(GLSL::Sampler2D, shadowRight);
	Uniform(GLSL::Sampler2D, shadowFront);
	Uniform(GLSL::Sampler2D, shadowBack);
	Uniform(GLSL::Sampler2D, shadowTop);
	Uniform(GLSL::Sampler2D, shadowBottom);

	void SetInputBuffer(const std::shared_ptr<GraphicsEngine::FrameBuffer>& buffer = nullptr);
};

class PhongForwardProgram : public ShaderProgram
{
private:
	InitializeProgram("PhongForwardProgram");
	AttachShader(phongVert);
	AttachShader(phongForwardFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);
	Uniform(GLSL::Matrix, objectTransform);
	Uniform(GLSL::Matrix, cameraTransform);
	Uniform(GLSL::Matrix, objectRotation);

	Uniform(GLSL::Vec4, color);
	Uniform(GLSL::Vec4, glowColor);
	Uniform(GLSL::Vec3, specularColor);

	Uniform(GLSL::Bool, textureEnabled);
	Uniform(GLSL::Bool, compressedNormalMap);
	Uniform(GLSL::Bool, normalMapEnabled);
	Uniform(GLSL::Bool, specularMapEnabled);
	Uniform(GLSL::Bool, blendTexture);
	Uniform(GLSL::Sampler2D, textureData);
	Uniform(GLSL::Sampler2D, normalMapData);
	Uniform(GLSL::Sampler2D, specularMapData);
	Uniform(GLSL::Vec4, textureColor);

	Uniform(GLSL::Bool, singleCubeMapped);
	Uniform(GLSL::Vec3, boxScale);
	Uniform(GLSL::Vec2, uvScale);
	Uniform(GLSL::Vec2, uvOffset);

	// material
	Uniform(GLSL::Vec3, materialDiffuse);
	Uniform(GLSL::Vec3, materialSpecular);
	Uniform(GLSL::Vec3, materialAmbient);
	Uniform(GLSL::Vec3, materialEmissive);
	Uniform(GLSL::Int, materialShininess);

	// global lighting
	Uniform(GLSL::Float, globalBrightness);
	Uniform(GLSL::Vec3, globalLightDirection);
	Uniform(GLSL::Vec3, globalDiffuse);
	Uniform(GLSL::Vec3, globalSpecular);
	Uniform(GLSL::Vec3, globalAmbient);
	Uniform(GLSL::Vec3, globalEmissive);

	// light object(s)
	Uniform(GLSL::Vec3Array, attenuation);
	Uniform(GLSL::Vec3Array, lightPosition);
	Uniform(GLSL::Vec3Array, lightDirection);
	Uniform(GLSL::Vec3Array, lightDiffuse);
	Uniform(GLSL::Vec3Array, lightSpecular);
	Uniform(GLSL::Vec3Array, lightAmbient);
	Uniform(GLSL::Vec2Array, spotlightAngles); // { inner, outer }
	Uniform(GLSL::IntArray, spotlightFalloff);
	Uniform(GLSL::IntArray, lightType);
	Uniform(GLSL::Int, activeLights);

	Uniform(GLSL::Bool, useOffsetMap);
	Uniform(GLSL::Float, offsetAmplitude);
	Uniform(GLSL::Vec2, offsetMinCoord);
	Uniform(GLSL::Vec2, offsetMaxCoord);
	Uniform(GLSL::Sampler2D, offsetMap);

	void SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);
	void SetNormalMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 1);
	void SetSpecularMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 2);
	void SetMaterial(const std::shared_ptr<GraphicsEngine::Material>& material);
};

class DepthTraceProgram : public ShaderProgram
{
private:
	InitializeProgram("DepthTraceProgram");
	AttachShader(depthTraceVert);
	AttachShader(depthTraceFrag);
	LinkProgram();
public:
	Uniform(GLSL::Matrix, transform);
	Uniform(GLSL::Matrix, objectTransform);
};

class ShadowMapProgram : public ShaderProgram
{
private:
	InitializeProgram("ShadowMapProgram");
	AttachShader(shadowMapVert);
	AttachShader(shadowMapFrag);
	LinkProgram();
public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Sampler2D, textureData);

	Uniform(GLSL::Vec4, objectZTransform);
	Uniform(GLSL::Vec4, color);
	Uniform(GLSL::Vec4, textureColor);

	Uniform(GLSL::Bool, textureEnabled);
	Uniform(GLSL::Bool, blendTexture);

	void SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);
};

class SkyboxProgram : public ShaderProgram
{
private:
	InitializeProgram("ShadowMapProgram");
	AttachShader(skyBoxVert);
	AttachShader(skyBoxFrag);
	LinkProgram();
public:
	Uniform(GLSL::Vec3, cameraRight);
	Uniform(GLSL::Vec3, cameraUp);
	Uniform(GLSL::Vec3, cameraFront);

	Uniform(GLSL::Float, skyBrightness);
	Uniform(GLSL::Float, skyBackgroundBrightness);
	Uniform(GLSL::Vec4, skyColor);
	Uniform(GLSL::Vec4, skyBackgroundColor);

	Uniform(GLSL::Sampler2D, skyLeft);
	Uniform(GLSL::Sampler2D, skyRight);
	Uniform(GLSL::Sampler2D, skyFront);
	Uniform(GLSL::Sampler2D, skyBack);
	Uniform(GLSL::Sampler2D, skyTop);
	Uniform(GLSL::Sampler2D, skyBottom);
};

class BlurProgram : public ShaderProgram
{
private:
	InitializeProgram("BlurProgram");
	AttachShader(screenVert);
	AttachShader(blurFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);
	Uniform(GLSL::Vec2, blurDirection);

	Uniform(GLSL::Sampler2D, textureData);

	Uniform(GLSL::Int, blurRadius);

	Uniform(GLSL::FloatArray, offsets);
	Uniform(GLSL::FloatArray, weights);

	void SetBlurRadius(int radius, float sigma);

	static float Gauss(float sigma, float x);
	static float Gauss2(float sigma, float a, float b);

private:
	typedef std::vector<float> FloatVector;

	const int MaxSize = 50;
	float Cutoff = 1.0f / 255;
	FloatVector WeightStorage, ClippedWeightStorage, OffsetStorage;
};

class LuminescenceProgram : public ShaderProgram
{
private:
	InitializeProgram("LuminescenceProgram");
	AttachShader(screenVert);
	AttachShader(luminescenceFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);
	Uniform(GLSL::Vec2, bounds);

	Uniform(GLSL::Bool, firstPass);
	Uniform(GLSL::Bool, finalPass);

	Uniform(GLSL::Int, luminescenceMode);

	Uniform(GLSL::Sampler2D, inputData);
};

class ToneMapProgram : public ShaderProgram
{
private:
	InitializeProgram("ToneMapProgram");
	AttachShader(screenVert);
	AttachShader(toneMapFrag);
	LinkProgram();

public:
	Uniform(GLSL::Int, rangeFittingMode);

	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Float, exposure);
	Uniform(GLSL::Float, burnoutCutoff);

	Uniform(GLSL::Int, luminescenceMode);

	Uniform(GLSL::Sampler2D, inputData);
	Uniform(GLSL::Sampler2D, luminescenceData);
};

class DepthUpdateProgram : public ShaderProgram
{
private:
	InitializeProgram("DepthUpdateProgram");
	AttachShader(screenVert);
	AttachShader(depthUpdateFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);
	Uniform(GLSL::Vec2, projectionData);

	Uniform(GLSL::Sampler2D, inputPosition);
};

class ScreenProgram : public ShaderProgram
{
private:
	InitializeProgram("ScreenProgram");
	AttachShader(screenVert);
	AttachShader(screenFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Bool, useTexture);
	Uniform(GLSL::Bool, blendTexture);
	Uniform(GLSL::Bool, useClippingMask);

	Uniform(GLSL::Vec2, uvScale);
	Uniform(GLSL::Vec2, uvOffset);
	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Vec4, color);
	Uniform(GLSL::Vec4, textureColor);

	Uniform(GLSL::Sampler2D, textureData);
	Uniform(GLSL::Sampler2D, clippingMask);

	void SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);
	void SetClippingMask(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);
};

class NormalMapGenProgram : public ShaderProgram
{
private:
	InitializeProgram("NormalMapGenProgram");
	AttachShader(screenVert);
	AttachShader(normalMapGenFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Float, scale);

	Uniform(GLSL::Sampler2D, heightMap);
};

class PerlinGradientProgram : public ShaderProgram
{
private:
	InitializeProgram("PerlinGradientProgram");
	AttachShader(screenVert);
	AttachShader(perlinGradientFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, offset);
};

class NoiseProgram : public ShaderProgram
{
private:
	InitializeProgram("NoiseProgram");
	AttachShader(screenVert);
	AttachShader(noiseFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, offset);
};

class AutomataProgram : public ShaderProgram
{
private:
	InitializeProgram("AutomataProgram");
	AttachShader(screenVert);
	AttachShader(automataFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, size);

	Uniform(GLSL::Sampler2D, inputTexture);
};

class NebulaProgram : public ShaderProgram
{
private:
	InitializeProgram("NebulaProgram");
	AttachShader(screenVert);
	AttachShader(nebulaFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Float, sigma);
	Uniform(GLSL::Float, maskWeight);
	Uniform(GLSL::Float, circleMaskWeight);
	
	Uniform(GLSL::Vec2, resolution);
	Uniform(GLSL::Vec2, distortion);
	Uniform(GLSL::Vec2, circleMaskOffset);
	
	Uniform(GLSL::Vec2, gradient1Size);
	Uniform(GLSL::Vec2, gradient2Size);
	Uniform(GLSL::Vec2, gradient3Size);
	Uniform(GLSL::Vec2, gradient1Scale);
	Uniform(GLSL::Vec2, gradient2Scale);
	Uniform(GLSL::Vec2, gradient3Scale);
	Uniform(GLSL::Vec2, gradient1Offset);
	Uniform(GLSL::Vec2, gradient2Offset);
	Uniform(GLSL::Vec2, gradient3Offset);

	Uniform(GLSL::Vec4, nebulaColor);
	
	Uniform(GLSL::Sampler2D, gradient1Data);
	Uniform(GLSL::Sampler2D, gradient2Data);
	Uniform(GLSL::Sampler2D, gradient3Data);
	Uniform(GLSL::Sampler2D, maskData);
};

class BlendProgram : public ShaderProgram
{
private:
	InitializeProgram("BlendProgram");
	AttachShader(screenVert);
	AttachShader(blendFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Bool, subtractMode);

	Uniform(GLSL::Vec2, resolution);
	Uniform(GLSL::Vec2, textureSize);
	Uniform(GLSL::Vec2, texturePosition);

	Uniform(GLSL::Vec4, color);

	Uniform(GLSL::Sampler2D, textureData);
	Uniform(GLSL::Sampler2D, backgroundData);
};

class BlurBlendProgam : public ShaderProgram
{
private:
	InitializeProgram("BlurBlendProgram");
	AttachShader(screenVert);
	AttachShader(blurBlendFrag);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Sampler2D, textureData);
	Uniform(GLSL::Sampler2D, blurData);
};

class ClippingMaskProgram : public ShaderProgram
{
private:
	InitializeProgram("ClippingMaskProgram");
	AttachShader(screenVert);
	AttachShader(clippingMask);
	LinkProgram();

public:
	Uniform(GLSL::Matrix, transform);

	Uniform(GLSL::Sampler2D, textureData);

	Uniform(GLSL::Vec2, resolution);

	Uniform(GLSL::Bool, hasParent);
};
