#include "ShaderLoader.h"

#define LoadShader(filePath, name, type) Shaders.push_back(new Shader(filePath, name, ShaderType::type))

ShaderLoader::ShaderLoader() : Shaders()
{
	LoadShader("./assets/shaders/screen.vert", "screenVert", Vertex);
	LoadShader("./assets/shaders/blur.frag", "blurFrag", Fragment);
	LoadShader("./assets/shaders/screen.frag", "screenFrag", Fragment);
	LoadShader("./assets/shaders/blend.frag", "blendFrag", Fragment);
	LoadShader("./assets/shaders/blurBlend.frag", "blurBlendFrag", Fragment);
	LoadShader("./assets/shaders/clippingMask.frag", "clippingMask", Fragment);
	LoadShader("./assets/shaders/luminescence.frag", "luminescenceFrag", Fragment);
	LoadShader("./assets/shaders/toneMap.frag", "toneMapFrag", Fragment);

	LoadShader("./assets/shaders/waterSim.frag", "waterSimFrag", Fragment);
	LoadShader("./assets/shaders/waterMatrix.frag", "waterMatrixFrag", Fragment);

	LoadShader("./assets/shaders/phong.vert", "phongVert", Vertex);
	LoadShader("./assets/shaders/phong.frag", "phongFrag", Fragment);
	LoadShader("./assets/shaders/phongForward.frag", "phongForwardFrag", Fragment);
	LoadShader("./assets/shaders/phongOutput.frag", "phongOutputFrag", Fragment);
	LoadShader("./assets/shaders/phongOutputStencil.frag", "phongOutputStencilFrag", Fragment);
	LoadShader("./assets/shaders/depthUpdate.frag", "depthUpdateFrag", Fragment);

	LoadShader("./assets/shaders/perlinGradient.frag", "perlinGradientFrag", Fragment);
	LoadShader("./assets/shaders/noise.frag", "noiseFrag", Fragment);
	LoadShader("./assets/shaders/automata.frag", "automataFrag", Fragment);
	LoadShader("./assets/shaders/nebula.frag", "nebulaFrag", Fragment);

	LoadShader("./assets/shaders/shadowMap.vert", "shadowMapVert", Vertex);
	LoadShader("./assets/shaders/shadowMap.frag", "shadowMapFrag", Fragment);

	LoadShader("./assets/shaders/depthTrace.vert", "depthTraceVert", Vertex);
	LoadShader("./assets/shaders/depthTrace.frag", "depthTraceFrag", Fragment);

	LoadShader("./assets/shaders/skyBox.vert", "skyBoxVert", Vertex);
	LoadShader("./assets/shaders/skyBox.frag", "skyBoxFrag", Fragment);

	LoadShader("./assets/shaders/normalMapGen.frag", "normalMapGenFrag", Fragment);
}

ShaderCleaner::ShaderCleaner(ShaderLoader* loader)
{
	for (int i = 0; i < int(loader->Shaders.size()); ++i)
		delete loader->Shaders[i];

	loader->Shaders.clear();
}
