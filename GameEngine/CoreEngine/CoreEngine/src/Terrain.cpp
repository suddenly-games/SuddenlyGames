#include "Terrain.h"

#include <sstream>
#include <filesystem>

#include "Transform.h"
#include "Graphics.h"
#include "ChunkTree.h"
#include "Camera.h"
#include "Chunk.h"
#include "TerrainMaterial.h"
#include "Material.h"

namespace Engine
{
	Terrain::~Terrain()
	{
		if (Head != nullptr)
			ChunkTreeAllocator.Destroy<ChunkTree>(Head);

		Head = nullptr;
	}

	void Terrain::Initialize()
	{
		auto materialProperties = Engine::Create<GraphicsEngine::Material>();

		materialProperties->Diffuse.Set(0.5f, 0.5, 0.5, 1);
		materialProperties->Specular.Set(0.5f, 0.5, 0.5, 1);
		materialProperties->Ambient.Set(0.5f, 0.5, 0.5, 1);
		materialProperties->Emission.Set(0, 0, 0, 1);
		materialProperties->Shininess = 50;
		materialProperties->SetParent(This.lock());

		MaterialProperties = materialProperties;
	}

	bool Terrain::CanLoadChunk(const Coordinates& chunk) const
	{
		if (DataDirectory != "")
		{
			std::filesystem::path path(DataDirectory);
			std::filesystem::path chunkFile(path / GetChunkPath(chunk));

			if (std::filesystem::is_directory(path) && std::filesystem::is_regular_file(chunkFile))
				return true;
		}

		if (GenerateChunk != nullptr)
			return true;

		return false;
	}

	std::shared_ptr<Chunk> Terrain::LoadChunk(const Coordinates& chunk)
	{
		if (Head == nullptr)
		{
			int gridScale = 1;

			ChunkTree::ExpandExtents(chunk, gridScale);

			Head = ChunkTreeAllocator.Create<ChunkTree>(0, 0, gridScale, &ChunkTreeAllocator, &ChunkColumnAllocator);
		}

		Head = Head->Expand(chunk);

		std::shared_ptr<Transform> transform = Engine::Create<Transform>();

		transform->SetParent(This.lock());
		transform->Name = GetChunkPath(chunk);

		std::shared_ptr<Chunk> newChunk = Head->CreateChunk(chunk);

		newChunk->SetTerrain(This.lock()->Cast<Terrain>(), chunk);
		newChunk->SetParent(transform);

		std::filesystem::path path(DataDirectory);
		std::filesystem::path chunkFile(path / GetChunkPath(chunk));

		if (std::filesystem::is_directory(path) && std::filesystem::is_regular_file(chunkFile))
			newChunk->Load();
		else if (GenerateChunk != nullptr)
			GenerateChunk(chunk, newChunk);

		return newChunk;
	}

	void Terrain::UnloadChunk(const Coordinates& chunk)
	{
		if (Head == nullptr)
			return;

		bool cleanUp = Head->RemoveChunk(chunk);

		if (cleanUp)
		{
			ChunkTreeAllocator.Destroy<ChunkTree>(Head);

			Head = nullptr;
		}
	}

	std::shared_ptr<TerrainMaterial> Terrain::GetMaterial(unsigned short ID) const
	{
		return nullptr;
	}

	std::string Terrain::GetChunkPath(const Coordinates& chunk) const
	{
		std::stringstream path;

		path << "c_" << chunk.X << "_" << chunk.Y << "_" << chunk.Z << ".cnk";

		return path.str();
	}

	Enum::ChunkStatus Terrain::GetChunkStatus(const Coordinates& chunk) const
	{
		std::shared_ptr<Chunk> chunkObject = GetChunk(chunk);

		if (chunkObject != nullptr)
			return chunkObject->GetStatus()->State;

		return Enum::ChunkStatus::Unloaded;
	}

	Coordinates Terrain::GetChunkCoordinates(const Coordinates& cell) const
	{
		Coordinates offset = GetCellCoordinates(cell);

		return Coordinates((cell.X - offset.X) / 16, (cell.Y - offset.Y) / 16, (cell.Z - offset.Z) / 16);
	}

	Coordinates Terrain::GetCellCoordinates(const Coordinates& cell) const
	{
		return Coordinates((cell.X % 16 + 16) & 0xF, (cell.Y % 16 + 16) & 0xF, (cell.Z % 16 + 16) & 0xF);;
	}

	std::shared_ptr<Chunk> Terrain::GetChunk(const Coordinates& chunk) const
	{
		if (Head != nullptr)
			return Head->GetChunk(chunk);

		return nullptr;
	}

	std::shared_ptr<Chunk> Terrain::GetChunkFromCell(const Coordinates& cell) const
	{
		return GetChunk(GetChunkCoordinates(cell));
	}

	CellData Terrain::GetCell(const Coordinates& cell) const
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			return chunk->GetCell(GetCellCoordinates(cell));

		return CellData();
	}

	unsigned short Terrain::GetMaterialID(const Coordinates& cell) const
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			return chunk->GetMaterialID(GetCellCoordinates(cell));

		return 0;
	}

	void Terrain::SetCell(const Coordinates& cell, const CellData& data)
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			chunk->SetCell(GetCellCoordinates(cell), data);
	}

	void Terrain::SetCell(const Coordinates& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data)
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			chunk->SetCell(GetCellCoordinates(cell), occupancy, material, data);
	}

	void Terrain::SetOccupancy(const Coordinates& cell, float occupancy)
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			chunk->SetOccupancy(GetCellCoordinates(cell), occupancy);
	}

	void Terrain::SetMaterial(const Coordinates& cell, const std::shared_ptr<TerrainMaterial>& material)
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			chunk->SetMaterial(GetCellCoordinates(cell), material);
	}

	void Terrain::SetData(const Coordinates& cell, const std::shared_ptr<Voxel>& data)
	{
		std::shared_ptr<Chunk> chunk = GetChunkFromCell(cell);

		if (chunk != nullptr)
			chunk->SetData(GetCellCoordinates(cell), data);
	}

	void Terrain::Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera)
	{
		std::shared_ptr<Transform> transform = GetComponent<Transform>();

		if (transform == nullptr)
			return;
		
		if (drawLiquid)
			Programs::PhongForward->SetMaterial(MaterialProperties.lock());
		else
			Programs::Phong->SetMaterial(MaterialProperties.lock());

		const Matrix3& transformation = transform->GetWorldTransformation();

		if (Head != nullptr && targetCamera->GetFrustum().Intersects(Head->GetBoundingBox().Transform(transformation)) != Enum::IntersectionType::Outside)
			Head->Draw(drawLiquid, targetCamera, transformation);
	}

	const ChunkTree* Terrain::GetHead() const
	{
		return Head;
	}
}