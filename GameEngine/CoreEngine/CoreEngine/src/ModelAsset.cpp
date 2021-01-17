#include "ModelAsset.h"

#include "MeshLoader.h"

namespace Engine
{
	ModelAsset::~ModelAsset()
	{
		Dying = true;

		if (MeshLoader::IsMeshLoaded(MeshID))
			MeshLoader::ReleaseAsset(*this);
	}

	int ModelAsset::GetMeshID() const
	{
		return MeshID;
	}

	bool ModelAsset::IsLoaded() const
	{
		return MeshLoader::IsMeshLoaded(MeshID);
	}

	bool ModelAsset::IsCore() const
	{
		return MeshLoader::IsMeshAllocated(MeshID) && MeshLoader::IsMeshCore(MeshID);
	}

	Vector3 ModelAsset::GetMinimumCorner() const
	{
		return MeshLoader::GetMeshMinimumCorner(MeshID);
	}

	Vector3 ModelAsset::GetMaximumCorner() const
	{
		return MeshLoader::GetMeshMaximumCorner(MeshID);
	}

	Vector3 ModelAsset::GetCenter() const
	{
		return MeshLoader::GetMeshCenter(MeshID);
	}

	Vector3 ModelAsset::GetSize() const
	{
		return MeshLoader::GetMeshSize(MeshID);
	}

	void ModelAsset::Load(const std::string& path, Enum::VertexMode mode)
	{
		MeshLoader::LoadAsset(This.lock()->Cast<ModelAsset>(), path, mode);
	}

	void ModelAsset::Unload()
	{
		MeshLoader::UnloadAsset(This.lock()->Cast<ModelAsset>());
	}

	void ModelAsset::Release()
	{
		if (MeshLoader::IsMeshLoaded(MeshID))
			MeshLoader::ReleaseAsset(This.lock()->Cast<ModelAsset>());

		SetParent(nullptr);
	}
}