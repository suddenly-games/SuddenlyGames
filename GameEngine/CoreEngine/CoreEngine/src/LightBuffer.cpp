#include "LightBuffer.h"

#include "Light.h"
#include "Camera.h"
#include "Graphics.h"

namespace GraphicsEngine
{
	void LightBuffer::PushLight(const std::shared_ptr<Light>& light)
	{
		Vector3 lightPosition = light->Position;
		Vector3 lightDirection = -light->Direction;

		auto currentCamera = CurrentCamera.lock();

		if (currentCamera != nullptr)
		{
			lightPosition = currentCamera->GetTransformationInverse() * lightPosition;
			lightDirection = currentCamera->GetTransformationInverse() * lightDirection;
		}

		Attenuation.push_back(light->Attenuation.X);
		Attenuation.push_back(light->Attenuation.Y);
		Attenuation.push_back(light->Attenuation.Z);

		Position.push_back(lightPosition.X);
		Position.push_back(lightPosition.Y);
		Position.push_back(lightPosition.Z);

		Direction.push_back(lightDirection.X);
		Direction.push_back(lightDirection.Y);
		Direction.push_back(lightDirection.Z);

		Diffuse.push_back(light->Diffuse.R);
		Diffuse.push_back(light->Diffuse.G);
		Diffuse.push_back(light->Diffuse.B);

		Specular.push_back(light->Specular.R);
		Specular.push_back(light->Specular.G);
		Specular.push_back(light->Specular.B);

		Ambient.push_back(light->Ambient.R);
		Ambient.push_back(light->Ambient.G);
		Ambient.push_back(light->Ambient.B);

		SpotlightAngles.push_back(cosf(light->InnerRadius));
		SpotlightAngles.push_back(cosf(light->OuterRadius));

		SpotlightFalloff.push_back(light->SpotlightFalloff);

		Type.push_back(light->Type);
	}

	void LightBuffer::Flush() const
	{
		Programs::PhongForward->attenuation.Set(Attenuation.data(), int(Type.size()));
		Programs::PhongForward->lightPosition.Set(Position.data(), int(Type.size()));
		Programs::PhongForward->lightDirection.Set(Direction.data(), int(Type.size()));
		Programs::PhongForward->lightDiffuse.Set(Diffuse.data(), int(Type.size()));
		Programs::PhongForward->lightSpecular.Set(Specular.data(), int(Type.size()));
		Programs::PhongForward->lightAmbient.Set(Ambient.data(), int(Type.size()));
		Programs::PhongForward->spotlightAngles.Set(SpotlightAngles.data(), int(Type.size()));
		Programs::PhongForward->spotlightFalloff.Set(SpotlightFalloff.data(), int(Type.size()));
		Programs::PhongForward->lightType.Set(Type.data(), int(Type.size()));
		Programs::PhongForward->activeLights.Set(int(Type.size()));
	}

	void LightBuffer::Clear()
	{
		Attenuation.clear();
		Position.clear();
		Direction.clear();
		Diffuse.clear();
		Specular.clear();
		Ambient.clear();
		SpotlightAngles.clear();
		SpotlightFalloff.clear();
		Type.clear();
	}
}
