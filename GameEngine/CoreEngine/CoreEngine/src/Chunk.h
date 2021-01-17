#pragma once

#include "Object.h"
#include "Vector3.h"
#include "Coordinates.h"
#include "CellData.h"
#include "VoxelTriangulation.h"
#include "ChunkStatusEnum.h"
#include "TerrainTypes.h"

namespace GraphicsEngine
{
	class Camera;
	class Material;
}

namespace Engine
{
	class Chunk : public Object
	{
	public:
		~Chunk();

		void Initialize();
		void Update(float delta);

		void SetTerrain(const std::shared_ptr<Object>& terrain, const Coordinates& chunk);

		void Save() const;
		void Load();
		void Regenerate();

		void Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera);

		std::shared_ptr<TerrainTypes::Status> GetStatus();

		CellData GetCell(const Coordinates& cell) const;
		CellData GetCell(const Vector3& cell) const { return GetCell(Coordinates(cell)); }
		Coordinates GetCoordinates() const { return Index; }
		unsigned short GetMaterialID(const Coordinates& cell) const;
		unsigned short GetMaterialID(const Vector3& cell) const { return GetMaterialID(Coordinates(cell)); }

		void SetCell(const Coordinates& cell, const CellData& data);
		void SetCell(const Coordinates& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data = nullptr);
		void SetOccupancy(const Coordinates& cell, float occupancy);
		void SetMaterial(const Coordinates& cell, const std::shared_ptr<TerrainMaterial>& material);
		void SetData(const Coordinates& cell, const std::shared_ptr<Voxel>& data);

		void SetCell(const Vector3& cell, const CellData& data) { return SetCell(Coordinates(cell), data); }
		void SetCell(const Vector3& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data = nullptr) { return SetCell(Coordinates(cell), occupancy, material, data); }
		void SetOccupancy(const Vector3& cell, float occupancy) { return SetOccupancy(Coordinates(cell), occupancy); }
		void SetMaterial(const Vector3& cell, const std::shared_ptr<TerrainMaterial>& material) { return SetMaterial(Coordinates(cell), material); }
		void SetData(const Vector3& cell, const std::shared_ptr<Voxel>& data) { return SetData(Coordinates(cell), data); }

		std::shared_ptr<Object> GetTerrain() const;

		template <typename T>
		std::shared_ptr<T> GetTerrain() const
		{
			return GetTerrain()->Cast<T>();
		}

	private:
		typedef std::vector<std::weak_ptr<Voxel>> VoxelVector;

		std::weak_ptr<TerrainTypes::Status> StatusHandle;
		std::weak_ptr<Object> ChunkModel;
		std::weak_ptr<GraphicsEngine::Material> MaterialProperties;
		std::weak_ptr<ModelAsset> ChunkMesh;
		std::weak_ptr<Object> ParentTerrain;
		Coordinates Index;
		TerrainTypes::VoxelData Data[16][16][16];
		VoxelVector Voxels;
		int OccupiedCells = 0;
		TerrainTypes::Status CurrentStatus;
		bool RegenerateGeometry = false;

		TerrainTypes::VoxelData& Get(const Coordinates& cell);
		const TerrainTypes::VoxelData& Get(const Coordinates& cell) const;

		std::shared_ptr<Chunk> FetchChunk(int chunkX, int chunkY, int chunkZ);
		void RegenerateNeighbors(bool changedLeft = true, bool changedBottom = true, bool changedBack = true);
		void AttemptRegeneration(int chunkX, int chunkY, int chunkZ);
		void Triangulate(VoxelTriangulation& triangulator, std::shared_ptr<Chunk>* chunks, bool* chunksCanLoad, int x, int y, int z);

		Instantiable;

		Inherits_Class(Object);

		Reflected(Chunk);
	};
}