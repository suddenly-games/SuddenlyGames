#include "ShadingOperation.h"

extern "C" {
#include <math.h>
}

#include "Graphics.h"
#include "Textures.h"
#include "Light.h"
#include "FrameBuffer.h"

namespace GraphicsEngine
{
	void ShadingOperation::Initialize()
	{
		auto shadowCamera = Engine::Create<Camera>();
		auto shadowScene = Engine::Create<Scene>();

		shadowCamera->SetParent(This.lock());
		shadowScene->SetParent(This.lock());

		ShadowCamera = shadowCamera;
		ShadowScene = shadowScene;

		shadowScene->CurrentCamera = ShadowCamera;

		int width = 2048;
		int height = 2048;

		auto rightMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));
		auto leftMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));
		auto topMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));
		auto bottomMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));
		auto frontMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));
		auto backMap = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));

		auto parent = This.lock();

		rightMap->SetParent(parent);
		leftMap->SetParent(parent);
		topMap->SetParent(parent);
		bottomMap->SetParent(parent);
		frontMap->SetParent(parent);
		backMap->SetParent(parent);

		RightMap = rightMap;
		LeftMap = leftMap;
		TopMap = topMap;
		BottomMap = bottomMap;
		FrontMap = frontMap;
		BackMap = backMap;
	}

	void ShadingOperation::Update(float)
	{
		if (CurrentScene.expired())
			return;

		CurrentScene.lock()->RefreshWatches();
	}

	void ShadingOperation::Render()
	{
		if (CurrentCamera.expired() || CurrentScene.expired())
			return;

		glEnable(GL_BLEND); CheckGLErrors();
		glEnable(GL_DEPTH_TEST); CheckGLErrors();
		glDepthMask(GL_FALSE); CheckGLErrors();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); CheckGLErrors();

		Programs::PhongOutput->Use();
		Programs::PhongOutput->SetInputBuffer(SceneBuffer.lock());
		Programs::PhongOutput->resolution.Set(Resolution);
		Programs::PhongOutput->shadowsEnabled.Set(false);

		Programs::PhongOutput->shadowLeft.Set(LeftMap.lock()->GetTexture(), 8);
		Programs::PhongOutput->shadowRight.Set(RightMap.lock()->GetTexture(), 9);
		Programs::PhongOutput->shadowFront.Set(FrontMap.lock()->GetTexture(), 10);
		Programs::PhongOutput->shadowBack.Set(BackMap.lock()->GetTexture(), 11);
		Programs::PhongOutput->shadowTop.Set(TopMap.lock()->GetTexture(), 12);
		Programs::PhongOutput->shadowBottom.Set(BottomMap.lock()->GetTexture(), 13);

		auto currentCamera = CurrentCamera.lock();

		if (GlobalLight.expired())
		{
			Programs::PhongOutput->lightBrightness.Set(1);
			Programs::PhongOutput->attenuation.Set(1, 0, 0);
			Programs::PhongOutput->lightPosition.Set(0, 0, 0);
			Programs::PhongOutput->lightDirection.Set(-currentCamera->GetTransformationInverse().UpVector());
			Programs::PhongOutput->lightDiffuse.Set(0.5f, 0.5f, 0.5f);
			Programs::PhongOutput->lightSpecular.Set(1, 1, 1);
			Programs::PhongOutput->lightAmbient.Set(0.5f, 0.5f, 0.5f);
			Programs::PhongOutput->spotlightAngles.Set(0, 0);
			Programs::PhongOutput->spotlightFalloff.Set(0);
			Programs::PhongOutput->lightType.Set(0);
		}
		else
		{
			auto globalLight = GlobalLight.lock();

			Programs::PhongOutput->lightBrightness.Set(globalLight->Brightness);
			Programs::PhongOutput->attenuation.Set(globalLight->Attenuation);
			Programs::PhongOutput->lightPosition.Set(globalLight->Position);
			Programs::PhongOutput->lightDirection.Set(currentCamera->GetTransformationInverse() * -globalLight->Direction);
			Programs::PhongOutput->lightDiffuse.Set(globalLight->Diffuse);
			Programs::PhongOutput->lightSpecular.Set(globalLight->Specular);
			Programs::PhongOutput->lightAmbient.Set(globalLight->Ambient);
			Programs::PhongOutput->spotlightAngles.Set(globalLight->InnerRadius, globalLight->OuterRadius);
			Programs::PhongOutput->spotlightFalloff.Set(globalLight->SpotlightFalloff);
			Programs::PhongOutput->lightType.Set(globalLight->Type);
		}

		Programs::PhongOutput->transform.Set(Matrix3().Scale(1, 1, 1));

		Programs::PhongOutput->CoreMeshes.Square->Draw();

		glEnable(GL_STENCIL_TEST); CheckGLErrors();
		glDepthFunc(GL_GEQUAL); CheckGLErrors();

		auto currentScene = CurrentScene.lock();

		for (int i = 0; i < currentScene->GetLights(); ++i)
		{
			std::shared_ptr<Light> light = currentScene->GetLight(i);

			if (!light->Enabled || light->AreShadowsEnabled())
				continue;

			if (currentCamera->GetFrustum().Intersects(light->GetBoundingBox()) == Enum::IntersectionType::Outside)
				continue;

			Draw(light);
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE); CheckGLErrors();

		for (int i = 0; i < currentScene->GetLights(); ++i)
		{
			std::shared_ptr<Light> light = currentScene->GetLight(i);

			if (!light->Enabled || !light->AreShadowsEnabled())
				continue;

			if (currentCamera->GetFrustum().Intersects(light->GetBoundingBox()) == Enum::IntersectionType::Outside)
				continue;

			glDisable(GL_BLEND); CheckGLErrors();
			glDepthMask(GL_TRUE); CheckGLErrors();
			glDisable(GL_STENCIL_TEST); CheckGLErrors();
			glEnable(GL_DEPTH_TEST); CheckGLErrors();
			glDepthFunc(GL_LEQUAL); CheckGLErrors();
			glCullFace(GL_FRONT); CheckGLErrors();

			Programs::DepthTrace->Use();

			DrawShadows(light, i);

			glCullFace(GL_BACK); CheckGLErrors();
			glEnable(GL_BLEND); CheckGLErrors();
			glEnable(GL_DEPTH_TEST); CheckGLErrors();
			glDepthMask(GL_FALSE); CheckGLErrors();
			glEnable(GL_STENCIL_TEST); CheckGLErrors();
			glDepthFunc(GL_GEQUAL); CheckGLErrors();

			Programs::PhongOutput->Use();

			LightBuffer.lock()->DrawTo();

			Draw(light);
		}

		glDepthMask(GL_TRUE); CheckGLErrors();
		glDisable(GL_STENCIL_TEST); CheckGLErrors();
		//glEnable(GL_DEPTH_TEST); CheckGLErrors();
		glDepthFunc(GL_LEQUAL); CheckGLErrors();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLErrors();
	}

	void ShadingOperation::Draw(const std::shared_ptr<Light>& light)
	{
		auto currentCamera = CurrentCamera.lock();

		Programs::PhongOutput->lightBrightness.Set(light->Brightness);
		Programs::PhongOutput->cameraTransform.Set(light->GetShadowMapInverseTransformation() * currentCamera->GetTransformation());
		Programs::PhongOutput->attenuation.Set(light->Attenuation);
		Programs::PhongOutput->lightPosition.Set(currentCamera->GetTransformationInverse() * light->Position);
		Programs::PhongOutput->lightDirection.Set(currentCamera->GetTransformationInverse() * -light->Direction);
		Programs::PhongOutput->lightDiffuse.Set(light->Diffuse);
		Programs::PhongOutput->lightSpecular.Set(light->Specular);
		Programs::PhongOutput->lightAmbient.Set(light->Ambient);
		Programs::PhongOutput->spotlightAngles.Set(cosf(light->InnerRadius), cosf(light->OuterRadius));
		Programs::PhongOutput->spotlightFalloff.Set(light->SpotlightFalloff);
		Programs::PhongOutput->lightType.Set(light->Type);

		const Mesh* mesh = nullptr;
		const Mesh* stencilMesh = nullptr;

		float lightRadius = 0;
		Matrix3 transform;

		if (light->Type == Enum::LightType::Directional)
		{
			transform = Matrix3().Scale(1, 1, 1);

			Programs::PhongOutput->shadowsEnabled.Set(false);
			Programs::PhongOutput->transform.Set(transform);

			mesh = Programs::PhongOutput->CoreMeshes.Square;
			stencilMesh = Programs::PhongOutputStencil->CoreMeshes.Square;
		}
		else
		{
			Dimensions shadowMapSize = light->GetShadowMapSize();

			Programs::PhongOutput->shadowsEnabled.Set(light->AreShadowsEnabled());
			Programs::PhongOutput->shadowDebugView.Set(light->ShadowDebugView);

			if (light->AreShadowsEnabled())
				Programs::PhongOutput->shadowScale.Set(float(shadowMapSize.Width) / 2048, float(shadowMapSize.Height) / 2048);

			lightRadius = light->GetRadius();

			Programs::PhongOutput->maxRadius.Set(lightRadius);

			lightRadius *= 1.1f;

			if (light->Type == Enum::LightType::Spot && light->OuterRadius <= PI / 2 + 0.001f)
			{
				Matrix3 rotation;
				float direction = 1;

				if (light->Direction == Vector3(0, -1, 0))
					rotation = Matrix3().RotatePitch(PI);//direction = -1;
				else if (light->Direction != Vector3(0, 1, 0))
					rotation = Matrix3().RotateYaw(atan2f(light->Direction.X, -light->Direction.Z)) * Matrix3().RotatePitch(-acosf(light->Direction.Y));

				transform = currentCamera->GetProjection() * Matrix3().Translate(light->Position) * Matrix3().Scale(-lightRadius, lightRadius, lightRadius) * rotation;

				Programs::PhongOutput->transform.Set(transform);

				if (light->OuterRadius <= PI / 4 + 0.001f)
				{
					mesh = Programs::PhongOutput->CoreMeshes.Cone;
					stencilMesh = Programs::PhongOutputStencil->CoreMeshes.Cone;
				}
				else
				{
					mesh = Programs::PhongOutput->CoreMeshes.HalfBoundingVolume;
					stencilMesh = Programs::PhongOutputStencil->CoreMeshes.HalfBoundingVolume;
				}
			}
			else
			{
				transform = currentCamera->GetProjectionMatrix() * Matrix3().Translate(
					currentCamera->GetTransformationInverse() * light->Position
				) * Matrix3().Scale(-lightRadius, lightRadius, lightRadius);

				Programs::PhongOutput->transform.Set(transform);

				mesh = Programs::PhongOutput->CoreMeshes.BoundingVolume;
				stencilMesh = Programs::PhongOutputStencil->CoreMeshes.BoundingVolume;
			}
		}

		glStencilMask(0xFF); CheckGLErrors();
		glStencilFunc(GL_ALWAYS, 1, 0xFF); CheckGLErrors();
		glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE); CheckGLErrors();

		Programs::PhongOutputStencil->Use();

		Programs::PhongOutputStencil->transform.Set(transform);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); CheckGLErrors();

		stencilMesh->Draw();

		glStencilFunc(GL_EQUAL, 1, 0xFF); CheckGLErrors();
		glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE); CheckGLErrors();

		Programs::PhongOutputStencil->transform.Set(transform * Matrix3().Scale(-1, 1, 1));

		glDepthFunc(GL_LEQUAL); CheckGLErrors();

		stencilMesh->Draw();

		Programs::PhongOutput->Use();

		glStencilMask(0x00); CheckGLErrors();
		glStencilFunc(GL_EQUAL, 1, 0xFF); CheckGLErrors();
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); CheckGLErrors();

		glDepthFunc(GL_GEQUAL); CheckGLErrors();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); CheckGLErrors();

		mesh->Draw();
	}

	void ShadingOperation::DrawShadows(const std::shared_ptr<Light>& light, int index)
	{
		float lightRadius = light->GetRadius() * 1.1f;

		Matrix3 backPanelTransform = Matrix3(0, 0, -lightRadius + 0.01f) * Matrix3().Scale(lightRadius, lightRadius, 0);

		//Matrix3 base = Matrix3().ExtractRotation(CurrentCamera->GetTransformation(), light->Position);

		Dimensions bufferSize = light->GetShadowMapSize();

		const AabbTree& watch = CurrentScene.lock()->GetWatched(index);

		auto shadowCamera = ShadowCamera.lock();

		shadowCamera->SetProperties(0.5f * PI, 1, 1e-1f, lightRadius);

		if (light->Type != Enum::LightType::Spot || light->OuterRadius > PI / 4 + 0.001f)
		{
			RightMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(-1, 0, 0)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}

		if (light->Type != Enum::LightType::Spot || light->OuterRadius > PI / 4 + 0.001f)
		{
			LeftMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}

		if (light->Type != Enum::LightType::Spot || light->OuterRadius > PI / 4 + 0.001f)
		{
			FrontMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}

		if (light->Type != Enum::LightType::Spot || light->OuterRadius > PI / 4 + 0.001f)
		{
			BackMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(-1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, -1)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}

		//if (light->Type != Enum::LightType::Spot || light->OuterRadius <= PI / 4 + 0.001f)
		{
			TopMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(1, 0, 0), Vector3(0, 0, 1), Vector3(0, -1, 0)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}

		if (light->Type != Enum::LightType::Spot || light->OuterRadius > 3 * PI / 4 + 0.001f)
		{
			BottomMap.lock()->DrawTo(0, 0, bufferSize.Width, bufferSize.Height);

			Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shadowCamera->SetTransformation(light->GetShadowMapTransformation() * Matrix3(Vector3(), Vector3(1, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0)));

			Scene::Draw(watch, false, shadowCamera);

			Programs::DepthTrace->transform.Set(shadowCamera->GetProjectionMatrix() * backPanelTransform);
			Programs::DepthTrace->objectTransform.Set(shadowCamera->GetTransformation() * backPanelTransform);

			Programs::DepthTrace->CoreMeshes.Cube->Draw();
		}
	}
}
