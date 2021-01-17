#include "MeshLoader.h"

#include <sstream>

#include "ObjParser.h"
#include "PlyParser.h"
#include "ModelAsset.h"

typename MeshLoader::LoaderVector MeshLoader::MeshLoaders = LoaderVector();
typename MeshLoader::MeshHeap MeshLoader::MeshContainer = MeshHeap();

MeshLoader::MeshLoader(const ShaderProgram* program) : Program(program)
{
	MeshLoaders.push_back(this);
}

MeshLoader::~MeshLoader()
{
	LoaderVector::iterator i;

	for (i = MeshLoaders.begin(); i != MeshLoaders.end() && *i != this; ++i);

	if (i != MeshLoaders.end())
		MeshLoaders.erase(i);
}

const Mesh* MeshLoader::GetMesh(int id)
{
	if (id >= MeshContainer.Size())
	{
		std::stringstream error;

		error << "Attempt to use out of bounds mesh ID: " << id;

		throw error.str();
	}

	if (int(Meshes.size()) <= id || Meshes[id] == nullptr)
	{
		for (int i = int(Meshes.size()); i <= id; ++i)
			Meshes.push_back(nullptr);

		Meshes[id] = new Mesh(&MeshContainer.GetNode(id).GetData().Data, Program, id);
	}

	return Meshes[id];
}

const Mesh* MeshLoader::operator[](int id)
{
	return GetMesh(id);
}

std::shared_ptr<Engine::ModelAsset> MeshLoader::NewAsset(const std::string& name)
{
	std::shared_ptr<Engine::ModelAsset> asset = Engine::Create<Engine::ModelAsset>();

	asset->Name = name;
	asset->MeshID = MeshContainer.RequestID();

	MeshHolder::AssetVector& assets = Assets();

	for (int i = int(assets.size()); i <= asset->MeshID; ++i)
		assets.push_back(std::weak_ptr<Engine::ModelAsset>());

	assets[asset->MeshID] = asset;

	return asset;
}

std::shared_ptr<Engine::ModelAsset> MeshLoader::NewAsset(const std::string& name, const MeshData& data, Enum::VertexMode mode)
{
	std::shared_ptr<Engine::ModelAsset> asset = NewAsset(name);

	LoadAsset(asset, data, mode);

	return asset;
}

std::shared_ptr<Engine::ModelAsset> MeshLoader::NewAsset(const std::string& name, const std::string& filePath, Enum::VertexMode mode)
{
	std::shared_ptr<Engine::ModelAsset> asset = NewAsset(name);

	LoadAsset(asset, filePath, mode);

	return asset;
}

void MeshLoader::ReleaseAsset(const std::shared_ptr<Engine::ModelAsset>& asset)
{
	if (asset == nullptr)
		throw "Attempt to release null mesh asset.";
	else if (!IsMeshAllocated(asset->MeshID))
		throw "Attempt to load unallocated mesh";

	MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

	if (info.IsCore)
		throw "Attempt to release core mesh";

	UnloadAsset(asset);

	MeshContainer.Release(asset->MeshID);

	MeshHolder::AssetVector& assets = Assets();

	if (int(assets.size()) > asset->MeshID)
		assets[asset->MeshID].reset();
}

void MeshLoader::ReleaseAsset(Engine::ModelAsset& asset)
{
	if (!asset.IsDying())
		throw "Attempt to release a live mesh";

	MeshInfo& info = MeshContainer.GetNode(asset.MeshID).GetData();

	UnloadAsset(asset.MeshID);

	MeshContainer.Release(asset.MeshID);

	MeshHolder::AssetVector& assets = Assets();

	if (int(assets.size()) > asset.MeshID)
		assets[asset.MeshID].reset();
}

void MeshLoader::LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, Enum::VertexMode mode)
{
	if (asset == nullptr)
		throw "Attempt to load null mesh asset.";
	else if (!IsMeshAllocated(asset->MeshID))
		throw "Attempt to load unallocated mesh";

	MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

	if (info.LoadedFile == "")
		throw "Attempt to load non-file mesh without a path";

	LoadAsset(asset, info.LoadedFile, mode);
}

void MeshLoader::LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, const MeshData& data, Enum::VertexMode mode)
{
	if (asset == nullptr)
		throw "Attempt to load null mesh asset.";
	else if (!IsMeshAllocated(asset->MeshID))
		throw "Attempt to load unallocated mesh";

	UnloadAsset(asset);

	MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

	info.IsWireframe = mode == Enum::VertexMode::Wireframe;
	info.LoadedFile = "";
	info.Mode = mode;
	info.Data = data;
	info.MinimumCorner = data.GetMinimumCorner();
	info.MaximumCorner = data.GetMaximumCorner();
}

void MeshLoader::LoadAsset(std::shared_ptr<Engine::ModelAsset> asset, const std::string& filePath, Enum::VertexMode mode)
{
	std::string extension = filePath.substr(filePath.size() - 4, 4);

	if (extension == ".obj")
	{
		ObjParser parser;

		parser.Parse(filePath);

		MeshData data(&parser, mode);

		LoadAsset(asset, data, mode);

		MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

		info.IsWireframe = mode == Enum::VertexMode::Wireframe;
		info.LoadedFile = filePath;
		info.MinimumCorner = data.GetMinimumCorner();
		info.MaximumCorner = data.GetMaximumCorner();
	}
	else if (extension == ".ply")
	{
		PlyParser parser;

		parser.Parse(filePath);

		MeshData data(&parser, mode);

		LoadAsset(asset, data, mode);

		MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

		info.IsWireframe = mode == Enum::VertexMode::Wireframe;
		info.LoadedFile = filePath;
		info.MinimumCorner = data.GetMinimumCorner();
		info.MaximumCorner = data.GetMaximumCorner();
	}
}

MeshLoader::MeshHolder::AssetVector& MeshLoader::Assets()
{
	static std::shared_ptr<MeshHolder> holder = std::shared_ptr<MeshHolder>(new MeshHolder(), [] (MeshHolder* data)
	{
		delete data;
	});

	static MeshHolder::AssetVector dummy;

	if (holder == nullptr)
		return dummy;

	return holder->Assets;
}

void MeshLoader::UnloadAsset(std::shared_ptr<Engine::ModelAsset> asset)
{
	if (asset == nullptr)
		throw "Attempt to unload null mesh asset.";
	else if (!IsMeshAllocated(asset->MeshID))
		throw "Attempt to unload unallocated mesh";

	MeshInfo& info = MeshContainer.GetNode(asset->MeshID).GetData();

	if (info.IsCore)
		throw "Attempt to release core mesh";
	
	UnloadAsset(asset->MeshID);
}

void MeshLoader::UnloadAsset(int id)
{
	MeshInfo& info = MeshContainer.GetNode(id).GetData();

	info.Data = MeshData();
	info.LoadedFile = "";
	
	for (int i = 0; i < int(MeshLoaders.size()); ++i)
	{
		if (MeshLoaders[i] != nullptr)
		{
			if (MeshLoaders[i] != nullptr && int(MeshLoaders[i]->Meshes.size()) > id && MeshLoaders[i]->Meshes[id] != nullptr)
			{
				delete MeshLoaders[i]->Meshes[id];

				MeshLoaders[i]->Meshes[id] = nullptr;
			}
		}
	}
}

bool MeshLoader::IsMeshAllocated(int id)
{
	if (id < 0 || id >= MeshContainer.Size())
		return false;

	if (!MeshContainer.NodeAllocated(id))
		return false;

	return true;
}

bool MeshLoader::IsMeshLoaded(int id)
{
	if (!IsMeshAllocated(id))
		return false;
	
	if (MeshContainer.GetNode(id).GetData().Data.VertexBuffer.size() == 0)
		return false;

	return true;
}

bool MeshLoader::IsMeshCore(int id)
{
	if (!IsMeshAllocated(id))
		return false;

	return MeshContainer.GetNode(id).GetData().IsCore;
}

bool MeshLoader::IsMeshWireframe(int id)
{
	if (!IsMeshAllocated(id))
		return false;

	return MeshContainer.GetNode(id).GetData().IsWireframe;
}

Vector3 MeshLoader::GetMeshMinimumCorner(int id)
{
	if (!IsMeshAllocated(id))
		return Vector3();

	return MeshContainer.GetNode(id).GetData().MinimumCorner;
}

Vector3 MeshLoader::GetMeshMaximumCorner(int id)
{
	if (!IsMeshAllocated(id))
		return Vector3();

	return MeshContainer.GetNode(id).GetData().MaximumCorner;
}

Vector3 MeshLoader::GetMeshCenter(int id)
{
	if (!IsMeshAllocated(id))
		return Vector3();

	const MeshInfo& data = MeshContainer.GetNode(id).GetData();

	return 0.5f * (data.MinimumCorner + data.MaximumCorner);
}

Vector3 MeshLoader::GetMeshSize(int id)
{
	if (!IsMeshAllocated(id))
		return Vector3();

	const MeshInfo& data = MeshContainer.GetNode(id).GetData();

	return data.MaximumCorner - data.MinimumCorner;
}


std::shared_ptr<Engine::ModelAsset> MeshLoader::GetAsset(int id)
{
	if (IsMeshAllocated(id))
		return Assets()[id].lock();

	return nullptr;
}

void MeshLoader::SetCore(int id)
{
	MeshContainer.GetNode(id).GetData().IsCore = true;
}

const MeshData* MeshLoader::GetMeshData(int id)
{
	if (IsMeshLoaded(id))
		return &MeshContainer.GetNode(id).GetData().Data;

	return nullptr;
}