#pragma once

#include "Coordinates.h"
#include "ChunkLoaderShapeEnum.h"
#include "Object.h"
#include "TerrainTypes.h"

namespace Engine
{
	class Terrain;
	class Chunk;

	class ChunkLoader : public Object
	{
	public:
		Enum::ChunkLoaderShape Shape = Enum::ChunkLoaderShape::Box;
		std::weak_ptr<Terrain> Target;

		void Initialize() {}
		void Update(float delta);

		void SetRange(const Coordinates& range);
		void SetRange(const Vector3& range) { SetRange(Coordinates(range)); }
		Coordinates GetRange() const;
		Coordinates GetCellCoordinates() const;
		Coordinates GetChunkCoordinates(const Coordinates& chunk = Coordinates()) const;
		Coordinates GetChunkCoordinates(const Vector3& chunk) const { return GetChunkCoordinates(Coordinates(chunk)); }
		Enum::ChunkStatus GetChunkStatus(const Coordinates& chunk) const;
		Enum::ChunkStatus GetChunkStatus(const Vector3& chunk) const { return GetChunkStatus(Coordinates(chunk)); }
		Enum::ChunkStatus GetLocalChunkStatus(const Coordinates& chunk) const;
		Enum::ChunkStatus GetLocalChunkStatus(const Vector3& chunk) const { return GetLocalChunkStatus(Coordinates(chunk)); }

	private:
		Coordinates Position = Coordinates(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		Coordinates Range = Vector3(0, 0, 0);
		std::vector<std::shared_ptr<TerrainTypes::Status>> CellReferences;

		void RefreshReferences();
		std::shared_ptr<TerrainTypes::Status> GetReference(const Coordinates& cell, Coordinates chunk) const;

		Instantiable;

		Inherits_Class(Object);

		Reflected(ChunkLoader);

	};
}