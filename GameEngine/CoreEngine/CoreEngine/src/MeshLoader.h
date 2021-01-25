#pragma once

#include <string>
#include <vector>
#include <map>

#include "Mesh.h"
#include "IdentifierHeap.h"
#include "Object.h"
#include "VertexModeEnum.h"
#include "MeshData.h"

namespace Engine
{
	class ModelAsset;
}

namespace GraphicsEngine
{
	class Texture;
}

class Mesh;
class ShaderProgram;

class MeshLoader : public Engine::Object
{
public:
	~MeshLoader();

	const Mesh* GetMesh(int id);

	const Mesh* operator[](int id);

	static std::shared_ptr<Engine::ModelAsset> NewAsset(const std::string& name);
	static std::shared_ptr<Engine::ModelAsset> NewAsset(const std::string& name, const MeshData& data, Enum::VertexMode mode = Enum::VertexMode::Seperate);
	static std::shared_ptr<Engine::ModelAsset> NewAsset(const std::string& name, const std::string& filePath, Enum::VertexMode mode = Enum::VertexMode::Seperate);

	static void ReleaseAsset(const std::shared_ptr<Engine::ModelAsset>& asset);
	static void ReleaseAsset(Engine::ModelAsset& asset);

	static void LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, Enum::VertexMode mode = Enum::VertexMode::Seperate);
	static void LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, const MeshData& data, Enum::VertexMode mode = Enum::VertexMode::Seperate);
	static void LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, const std::string& filePath, Enum::VertexMode mode = Enum::VertexMode::Seperate);

	static void UnloadAsset(std::shared_ptr<Engine::ModelAsset> asset);

	void SetProgram(const ShaderProgram* program);

	static std::shared_ptr<Engine::ModelAsset> GetAsset(int id);
	static bool IsMeshLoaded(int id);
	static bool IsMeshAllocated(int id);
	static bool IsMeshCore(int id);
	static bool IsMeshWireframe(int id);
	static Vector3 GetMeshMinimumCorner(int id);
	static Vector3 GetMeshMaximumCorner(int id);
	static Vector3 GetMeshCenter(int id);
	static Vector3 GetMeshSize(int id);

	static const MeshData* GetMeshData(int id);

	friend class Graphics;

	struct MeshHolder
	{
		typedef std::vector<std::weak_ptr<Engine::ModelAsset>> AssetVector;

		AssetVector Assets;
	};

private:
	struct MeshInfo
	{
		bool IsWireframe = false;
		bool IsCore = false;
		std::string LoadedFile;
		MeshData Data;
		Vector3 MinimumCorner;
		Vector3 MaximumCorner;

		Enum::VertexMode Mode = Enum::VertexMode::Seperate;
	};

	typedef std::vector<Mesh*> MeshVector;
	typedef std::vector<MeshLoader*> LoaderVector;
	typedef IDHeap<MeshInfo> MeshHeap;
	static void SetCore(int id);

	const ShaderProgram* Program = nullptr;
	MeshVector Meshes;
	static MeshHeap MeshContainer;
	static LoaderVector MeshLoaders;
	static MeshHolder::AssetVector& Assets();

	static void UnloadAsset(int id);

	Instantiable;

	Inherits_Class(Engine::Object);

	Reflected(MeshLoader);
};
