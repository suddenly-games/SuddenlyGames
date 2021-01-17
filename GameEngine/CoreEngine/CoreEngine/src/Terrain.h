#pragma once

#include "Object.h"
#include "Vector3.h"
#include "CellData.h"
#include "Coordinates.h"
#include "PageAllocator.h"
#include "ChunkStatusEnum.h"
#include "ChunkTree.h"

namespace GraphicsEngine
{
	class Material;
}

namespace Engine
{
	class Chunk;
	class Camera;
	class ChunkTree;
	class TerrainMaterial;

	class Terrain : public Object
	{
	public:
		typedef std::function<void(const Coordinates& coordinates, const std::shared_ptr<Chunk>& chunk)> GenerationCallback;
		
		GenerationCallback GenerateChunk;
		std::string DataDirectory;

		~Terrain();

		void Initialize();
		void Update(float delta) {}

		bool CanLoadChunk(const Coordinates& chunk) const;
		std::shared_ptr<Chunk> LoadChunk(const Coordinates& chunk);
		void UnloadChunk(const Coordinates& chunk);

		bool CanLoadChunk(const Vector3& chunk) const { return CanLoadChunk(Coordinates(chunk)); }

		std::shared_ptr<TerrainMaterial> GetMaterial(unsigned short ID) const;

		std::string GetChunkPath(const Coordinates& chunk) const;

		Enum::ChunkStatus GetChunkStatus(const Coordinates& chunk) const;

		Coordinates GetChunkCoordinates(const Coordinates& cell) const;
		Coordinates GetCellCoordinates(const Coordinates& cell) const;

		std::shared_ptr<Chunk> GetChunk(const Coordinates& chunk) const;
		std::shared_ptr<Chunk> GetChunkFromCell(const Coordinates& cell) const;
		
		CellData GetCell(const Coordinates& cell) const;
		unsigned short GetMaterialID(const Coordinates& cell) const;
		unsigned short GetMaterialID(const Vector3& cell) const { return GetMaterialID(Coordinates(cell)); }

		void SetCell(const Coordinates& cell, const CellData& data);
		void SetCell(const Coordinates& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data = nullptr);
		void SetOccupancy(const Coordinates& cell, float occupancy);
		void SetMaterial(const Coordinates& cell, const std::shared_ptr<TerrainMaterial>& material);
		void SetData(const Coordinates& cell, const std::shared_ptr<Voxel>& data);

		void Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera);

		Enum::ChunkStatus GetChunkStatus(const Vector3& chunk) const { return GetChunkStatus(Coordinates(chunk)); }

		Coordinates GetChunkCoordinates(const Vector3& cell) const { return GetChunkCoordinates(Coordinates(cell)); }
		Coordinates GetCellCoordinates(const Vector3& cell) const { return GetCellCoordinates(Coordinates(cell)); }

		std::shared_ptr<Chunk> GetChunk(const Vector3& chunk) const { return GetChunk(Coordinates(chunk)); }
		std::shared_ptr<Chunk> GetChunkFromCell(const Vector3& cell) const { return GetChunkFromCell(Coordinates(cell)); }

		CellData GetCell(const Vector3& cell) const { return GetCell(Coordinates(cell)); }

		void SetCell(const Vector3& cell, const CellData& data) { return SetCell(Coordinates(cell), data); }
		void SetCell(const Vector3& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data = nullptr) { return SetCell(Coordinates(cell), occupancy, material, data); }
		void SetOccupancy(const Vector3& cell, float occupancy) { return SetOccupancy(Coordinates(cell), occupancy); }
		void SetMaterial(const Vector3& cell, const std::shared_ptr<TerrainMaterial>& material) { return SetMaterial(Coordinates(cell), material); }
		void SetData(const Vector3& cell, const std::shared_ptr<Voxel>& data) { return SetData(Coordinates(cell), data); }

		const ChunkTree* GetHead() const;

		std::shared_ptr<GraphicsEngine::Material> GetMaterial() const { return MaterialProperties.lock(); }

	private:
		PageAllocator<sizeof(ChunkTree)> ChunkTreeAllocator;
		PageAllocator<sizeof(ChunkColumn)> ChunkColumnAllocator;

		ChunkTree* Head = nullptr;

		std::weak_ptr<GraphicsEngine::Material> MaterialProperties;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Terrain);

	};
}