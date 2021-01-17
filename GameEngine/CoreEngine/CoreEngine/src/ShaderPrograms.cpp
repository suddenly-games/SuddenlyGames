#include "ShaderPrograms.h"

#include "Graphics.h"
#include "FrameBuffer.h"
#include "Material.h"

extern "C" {
#include <math.h>
}

void PhongProgram::SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	textureData.Set(texture, sampler);
	textureEnabled.Set(texture != nullptr);
}

void PhongProgram::SetNormalMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	normalMapData.Set(texture, sampler);
	normalMapEnabled.Set(texture != nullptr);
}

void PhongProgram::SetSpecularMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	specularMapData.Set(texture, sampler);
	specularMapEnabled.Set(texture != nullptr);
}

void PhongProgram::SetMaterial(const std::shared_ptr<GraphicsEngine::Material>& material)
{
	if (CurrentMaterial.lock() != material && material != nullptr)
	{
		CurrentMaterial = material;

		materialDiffuse.Set(material->Diffuse);
		materialAmbient.Set(material->Ambient);
		materialSpecular.Set(material->Specular);
		materialEmissive.Set(material->Emission);
		materialShininess.Set(material->Shininess);
	}
}

void PhongOutputProgram::SetInputBuffer(const std::shared_ptr<GraphicsEngine::FrameBuffer>& buffer)
{
	if (buffer != nullptr)
	{
		inputPosition.Set(buffer->GetTexture(0), 3);
		inputNormal.Set(buffer->GetTexture(1), 4);
		inputDiffuse.Set(buffer->GetTexture(2), 5);
		inputSpecular.Set(buffer->GetTexture(3), 6);
		inputAmbient.Set(buffer->GetTexture(4), 7);
	}
	else
	{
		//inputPosition.Set(Engine::Null);
		//inputNormal.Set(Engine::Null);
		//inputDiffuse.Set(Engine::Null);
		//inputSpecular.Set(Engine::Null);
		//inputAmbient.Set(Engine::Null);
	}
}

void PhongOutputStencilProgram::SetInputBuffer(const std::shared_ptr<GraphicsEngine::FrameBuffer>& buffer)
{
	if (buffer != nullptr)
		inputPosition.Set(buffer->GetTexture(0), 3);
	else
		inputPosition.Set(nullptr);
}

void PhongForwardProgram::SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	textureData.Set(texture, sampler);
	textureEnabled.Set(texture != nullptr);
}

void PhongForwardProgram::SetNormalMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	normalMapData.Set(texture, sampler);
	normalMapEnabled.Set(texture != nullptr);
}

void PhongForwardProgram::SetSpecularMap(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	specularMapData.Set(texture, sampler);
	specularMapEnabled.Set(texture != nullptr);
}

void PhongForwardProgram::SetMaterial(const std::shared_ptr<GraphicsEngine::Material>& material)
{
	materialDiffuse.Set(material->Diffuse);
	materialAmbient.Set(material->Ambient);
	materialSpecular.Set(material->Specular);
	materialEmissive.Set(material->Emission);
	materialShininess.Set(material->Shininess);
}

void ShadowMapProgram::SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	textureData.Set(texture, sampler);
	textureEnabled.Set(texture != nullptr);
}

float BlurProgram::Gauss(float sigma, float x)
{
	return expf(-(x*x) / (2.0f * sigma*sigma));
}

float BlurProgram::Gauss2(float sigma, float a, float b)
{
	return (1.0f / 6) * (Gauss(sigma, a) + 4 * Gauss(sigma, 0.5f * (a + b)) + Gauss(sigma, b));
}

void BlurProgram::SetBlurRadius(int radius, float sigma)
{
	WeightStorage.clear();
	WeightStorage.resize(MaxSize);
	OffsetStorage.clear();
	OffsetStorage.resize(MaxSize);
	ClippedWeightStorage.clear();
	ClippedWeightStorage.resize(MaxSize);

	WeightStorage[0] = Gauss2(sigma, 0.5f, -0.5f);
	OffsetStorage[0] = 0;
	
	float total = WeightStorage[0];
	int i = 1;
	
	for (int x = 1; x < radius; x += 2)
	{
		if (x == radius - 1 && (x & 1) == 1)
		{
			WeightStorage[i] = Gauss2(sigma, x + 0.5f, x - 0.5f);
			OffsetStorage[i] = float(x);
	
			total += 2 * WeightStorage[i];
		}
		else
		{
			float weight1 = Gauss2(sigma, x + 0.5f, x - 0.5f);
			float weight2 = Gauss2(sigma, x + 1.5f, x + 0.5f);
	
			WeightStorage[i] = weight1 + weight2;
			OffsetStorage[i] = (x * weight1 + (x + 1) * weight2) / (weight1 + weight2);
		}
	
		++i;
	}
	
	for (int x = 0; x < i; ++x)
		WeightStorage[x] /= total;
	
	blurRadius.Set(i);
	
	offsets.Set(OffsetStorage.data(), i);
	weights.Set(WeightStorage.data(), i);
}

void ScreenProgram::SetTexture(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	textureData.Set(texture, sampler);

	useTexture.Set(texture != nullptr);
}

void ScreenProgram::SetClippingMask(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler)
{
	clippingMask.Set(texture, sampler);

	useClippingMask.Set(texture != nullptr);

	resolution.Set(Graphics::ActiveWindow->Resolution);
}
