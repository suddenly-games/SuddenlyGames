#pragma once

#include <map>
#include <set>

#include "Mesh.h"
#include "RGBA.h"
#include "ShaderPrograms.h"
#include "Scene.h"

namespace GraphicsEngine
{
	class Texture;
	class ModelAsset;
	class Camera;

	class Model : public SceneObject
	{
	public:
		virtual ~Model() {}

		void Initialize() {}
		void Update(float);

		bool BlendTexture = true;
		RGBA Color;
		RGBA TextureColor;
		RGBA GlowColor = RGBA(0, 0, 0, 1);
		Vector3 UVScale = Vector3(1, 1);
		Vector3 UVOffset;
		Vector3 BoxScale = Vector3(1, 1, 1);
		bool CubeMapped = false;
		bool FlipCubeMapV = false;
		bool CompressedNormalMap = false;
		bool UseOffsetMap = false;
		std::weak_ptr<Texture> NormalMap;
		std::weak_ptr<Texture> SpecularMap;
		std::weak_ptr<Texture> GlowMap;
		std::weak_ptr<Texture> CubeMapTop;
		std::weak_ptr<Texture> CubeMapBottom;
		std::weak_ptr<Texture> CubeMapLeft;
		std::weak_ptr<Texture> CubeMapRight;
		std::weak_ptr<Texture> CubeMapFront;
		std::weak_ptr<Texture> CubeMapBack;
		std::weak_ptr<Texture> DiffuseTexture;
		std::weak_ptr<Engine::ModelAsset> Asset;

		void Draw(const std::shared_ptr<Camera>& camera);
		std::string GetMeshName() const;
		bool IsTransparent() const;
		Aabb GetBoundingBox() const;
		bool HasMoved() const;
		bool IsStatic() const;

	private:
		void DrawMesh(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const;
		void DrawForward(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const;
		void DrawShadow(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const;
		void DrawDepth(const std::shared_ptr<Camera>& camera, const Mesh* mesh, const Matrix3& transformation) const;

		Instantiable;

		Inherits_Class(SceneObject);

		Reflected(Model);
	};
}
namespace Engine
{
	//Class_Inherits(GraphicsEngine::Model, GraphicsEngine::SceneObject);
}
