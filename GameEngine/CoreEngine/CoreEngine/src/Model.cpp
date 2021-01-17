#include "Model.h"

#include "Constants.h"
#include "Graphics.h"
#include "Transform.h"
#include "Camera.h"
#include "Texture.h"
#include "ModelAsset.h"

namespace GraphicsEngine
{
	void Model::Update(float)
	{
		if (Color.A <= 0.001f && (TextureColor.A <= 0.001f || DiffuseTexture.expired()))
			Visible = false;
	}

	void Model::Draw(const std::shared_ptr<Camera>& camera)
	{
		std::shared_ptr<Engine::Transform> transform = GetComponent<Engine::Transform>();

		if (transform == nullptr || MaterialProperties.expired() || Asset.expired())
			return;

		const ShaderProgram* program = ShaderProgram::GetCurrentProgram();

		const Mesh* mesh = ShaderProgram::GetCurrentProgram()->Meshes.GetMesh(Asset.lock()->GetMeshID());

		if (mesh != nullptr)
		{
			if (program == Programs::Phong)
				DrawMesh(camera, mesh, transform->GetWorldTransformation());
			else if (program == Programs::PhongForward)
				DrawForward(camera, mesh, transform->GetWorldTransformation());
			else if (program == Programs::ShadowMap)
				DrawShadow(camera, mesh, transform->GetWorldTransformation());
			else if (program == Programs::DepthTrace)
				DrawDepth(camera, mesh, transform->GetWorldTransformation());
		}
	}

	void Model::DrawMesh(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const
	{
		Matrix3 transform = camera->GetTransformationInverse() * transformation;
		Programs::Phong->transform.Set(camera->GetProjectionMatrix() * transform);
		//Programs::Phong->transform.Set(camera->GetProjectionMatrix());
		Programs::Phong->objectTransform.Set(transform);
		Programs::Phong->objectRotation.Set(Matrix3().ExtractRotation(transform));

		Programs::Phong->skyboxEnabled.Set(!CubeMapTop.expired());

		if (!CubeMapTop.expired())
		{
			Programs::Phong->cubeMapTop.Set(CubeMapTop.lock(), 3);
			Programs::Phong->cubeMapBottom.Set(CubeMapBottom.lock(), 4);
			Programs::Phong->cubeMapLeft.Set(CubeMapLeft.lock(), 5);
			Programs::Phong->cubeMapRight.Set(CubeMapRight.lock(), 6);
			Programs::Phong->cubeMapFront.Set(CubeMapFront.lock(), 7);
			Programs::Phong->cubeMapBack.Set(CubeMapBack.lock(), 8);
		}

		Programs::Phong->useOffsetMap.Set(UseOffsetMap);
		Programs::Phong->color.Set(Color);
		Programs::Phong->glowColor.Set(GlowColor);
		Programs::Phong->SetTexture(DiffuseTexture.lock());
		Programs::Phong->SetNormalMap(NormalMap.lock());
		Programs::Phong->SetSpecularMap(SpecularMap.lock());
		Programs::Phong->textureColor.Set(TextureColor);
		Programs::Phong->blendTexture.Set(BlendTexture);
		Programs::Phong->compressedNormalMap.Set(CompressedNormalMap);
		Programs::Phong->singleCubeMapped.Set(CubeMapped);
		Programs::Phong->flipCubeMapV.Set(FlipCubeMapV);
		Programs::Phong->boxScale.Set(BoxScale);
		Programs::Phong->uvScale.Set(UVScale);
		Programs::Phong->uvOffset.Set(UVOffset);
		//Programs::Phong->reflective.Set(Handle<GraphicsEngine::Material>(MaterialProperties)->Name == "Reflective");

		mesh->Draw();
	}

	void Model::DrawForward(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const
	{
		Matrix3 transform = camera->GetTransformationInverse() * transformation;
		Programs::PhongForward->transform.Set(camera->GetProjectionMatrix() * transform);
		//Programs::PhongForward->transform.Set(camera->GetProjectionMatrix());
		Programs::PhongForward->objectTransform.Set(transform);
		Programs::PhongForward->objectRotation.Set(Matrix3().ExtractRotation(transform));

		Programs::PhongForward->useOffsetMap.Set(UseOffsetMap);
		Programs::PhongForward->color.Set(Color);
		//Programs::PhongForward->specularColor.Set(specularColor);
		Programs::PhongForward->glowColor.Set(GlowColor);
		Programs::PhongForward->SetTexture(DiffuseTexture.lock());
		Programs::PhongForward->SetNormalMap(NormalMap.lock());
		Programs::PhongForward->SetSpecularMap(SpecularMap.lock());
		Programs::PhongForward->textureColor.Set(TextureColor);
		Programs::PhongForward->blendTexture.Set(BlendTexture);
		//Programs::PhongForward->flipCubeMapV.Set(flipCubeMapV);
		Programs::PhongForward->singleCubeMapped.Set(CubeMapped);
		Programs::PhongForward->compressedNormalMap.Set(CompressedNormalMap);
		Programs::PhongForward->boxScale.Set(BoxScale);
		Programs::PhongForward->uvScale.Set(UVScale);
		Programs::PhongForward->uvOffset.Set(UVOffset);

		mesh->Draw();
	}

	void Model::DrawShadow(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const
	{
		Matrix3 transform = camera->GetTransformationInverse() * transformation;
		Programs::ShadowMap->transform.Set(camera->GetProjectionMatrix() * transform);
		Programs::ShadowMap->objectZTransform.Set(transform.Data[2][0], transform.Data[2][1], transform.Data[2][2], transform.Data[2][3]);

		//Programs::ShadowMap->color.Set(Color);
		//Programs::ShadowMap->textureColor.Set(TextureColor);
		//Programs::ShadowMap->blendTexture.Set(BlendTexture);
		//Programs::ShadowMap->SetTexture(DiffuseTexture);
		//Programs::ShadowMap->blendTexture.Set(BlendTexture);

		mesh->Draw();
	}

	void Model::DrawDepth(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const
	{
		Matrix3 transform = camera->GetTransformationInverse() * transformation;

		Programs::DepthTrace->transform.Set(camera->GetProjectionMatrix() * transform);
		Programs::DepthTrace->objectTransform.Set(transform);

		mesh->Draw();
	}

	std::string Model::GetMeshName() const
	{
		if (Asset.expired())
			return "";

		return Asset.lock()->Name;
	}

	bool Model::IsTransparent() const
	{
		return Color.A != 1;
	}

	Aabb Model::GetBoundingBox() const
	{
		if (Asset.expired())
			return Aabb();

		const MeshData* data = MeshLoader::GetMeshData(Asset.lock()->GetMeshID());

		std::shared_ptr<Engine::Transform> transform = GetComponent<Engine::Transform>();

		if (transform == nullptr)
			return Aabb();

		return Aabb(data->GetMinimumCorner(), data->GetMaximumCorner()).Transform(transform->GetWorldTransformation());
	}

	bool Model::HasMoved() const
	{
		//WeakHandle<Engine::Transform> transform = GetComponent<Engine::Transform>();
		//
		//if (!transform.IsNull())
		//	return transform->HasMoved();

		return !IsStatic();
	}

	bool Model::IsStatic() const
	{
		std::shared_ptr<Engine::Transform> transform = GetComponent<Engine::Transform>();

		if (transform != nullptr)
			return transform->IsStatic;

		return true;
	}
}
