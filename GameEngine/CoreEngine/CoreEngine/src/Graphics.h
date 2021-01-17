#pragma once

#include "ShaderPrograms.h"
#include "ShaderLoader.h"
#include "Window.h"
#include "MeshData.h"

#define CheckGLErrors() Graphics::CheckErrors(__FILE__, __LINE__, "")

namespace Engine
{
	class ModelAsset;
	class FrameBuffer;
}

class Graphics
{
public:
	static Window* ActiveWindow;
	static Graphics* System;

	static void Initialize();
	static void Clean();
	static void SetClearColor(const RGBA& color);
	static void ClearScreen(GLbitfield mask);
	static void SetBlendMode(GLenum sMode, GLenum dMode);

private:
	ShaderLoader ShaderProgramLoader;

public:
	struct ShaderPrograms
	{
		PhongProgram Phong;
		PhongOutputProgram PhongOutput;
		PhongOutputStencilProgram PhongOutputStencil;
		PhongForwardProgram PhongForward;
		DepthUpdateProgram DepthUpdate;
		WaterMatrixProgram WaterMatrix;
		WaterSimProgram WaterSim;
		ShadowMapProgram ShadowMap;
		SkyboxProgram SkyBox;
		LuminescenceProgram Luminescence;
		ToneMapProgram ToneMap;
		DepthTraceProgram DepthTrace;
		PerlinGradientProgram PerlinGradient;
		NoiseProgram Noise;
		AutomataProgram Automata;
		NebulaProgram Nebula;
		BlurProgram Blur;
		ScreenProgram Screen;
		NormalMapGenProgram NormalMapGen;
		BlendProgram Blend;
		BlurBlendProgam BlurBlend;
		ClippingMaskProgram ClippingMask;
	};

	ShaderPrograms Programs;

	void LoadAssets();

	static void CheckErrors(const char* file, int line, const char* func);

private:
	ShaderCleaner ShaderProgramCleaner = ShaderCleaner(&ShaderProgramLoader);

	Graphics(); // no touchy

	MeshData SquareData;
	MeshData CubeData;
	MeshData WireCubeData;
	MeshData BoundingVolumeData;
	MeshData HalfBoundingVolumeData;
	MeshData ConeData;

	std::shared_ptr<Engine::ModelAsset> CoreSquare;
	std::shared_ptr<Engine::ModelAsset> CoreCube;
	std::shared_ptr<Engine::ModelAsset> CoreWireCube;
	std::shared_ptr<Engine::ModelAsset> CoreBoundingVolume;
	std::shared_ptr<Engine::ModelAsset> CoreHalfBoundingVolume;
	std::shared_ptr<Engine::ModelAsset> CoreCone;

	std::shared_ptr<Engine::ModelAsset> LoadCore(const std::string& name, const MeshData& data, const std::shared_ptr<Engine::Object>& parent);
};

class Programs
{
public:
	static PhongProgram* Phong;
	static PhongOutputProgram* PhongOutput;
	static PhongOutputStencilProgram* PhongOutputStencil;
	static PhongForwardProgram* PhongForward;
	static DepthUpdateProgram* DepthUpdate;
	static WaterMatrixProgram* WaterMatrix;
	static WaterSimProgram* WaterSim;
	static ShadowMapProgram* ShadowMap;
	static SkyboxProgram* SkyBox;
	static LuminescenceProgram* Luminescence;
	static ToneMapProgram* ToneMap;
	static DepthTraceProgram* DepthTrace;
	static PerlinGradientProgram* PerlinGradient;
	static NoiseProgram* Noise;
	static AutomataProgram* Automata;
	static NebulaProgram* Nebula;
	static BlurProgram* Blur;
	static ScreenProgram* Screen;
	static NormalMapGenProgram* NormalMapGen;
	static BlendProgram* Blend;
	static BlurBlendProgam* BlurBlend;
	static ClippingMaskProgram* ClippingMask;
};
