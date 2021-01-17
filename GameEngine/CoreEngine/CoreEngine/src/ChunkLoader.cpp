#include "ChunkLoader.h"

#include <algorithm>

#include "Transform.h"
#include "Chunk.h"
#include "Terrain.h"

namespace Engine
{
	void ChunkLoader::Update(float delta)
	{
		Coordinates position = GetChunkCoordinates();

		if (Position.X != position.X || Position.Y != position.Y || Position.Z != position.Z)
		{
			Position = position;

			RefreshReferences();
		}
	}

	void ChunkLoader::SetRange(const Coordinates& range)
	{
		if (range.X != Range.X || range.Y != Range.Y || range.Z != Range.Z)
		{
			Coordinates size(2 * range.X + 1, 2 * range.Y + 1, 2 * range.Z + 1);

			Range = range;

			CellReferences.resize(size.X * size.Y * size.Z);

			RefreshReferences();
		}

		Range = range;
	}

	Coordinates ChunkLoader::GetRange() const
	{
		return Range;
	}

	Coordinates ChunkLoader::GetCellCoordinates() const
	{
		if (Target.expired())
			return Coordinates();

		std::shared_ptr<Transform> transform = GetComponent<Transform>();

		if (transform == nullptr)
			return Coordinates();

		std::shared_ptr<Transform> terrainTransform = Target.lock()->GetComponent<Transform>();
		
		return terrainTransform->GetWorldTransformationInverse() * transform->GetWorldPosition();
	}

	Coordinates ChunkLoader::GetChunkCoordinates(const Coordinates& chunk) const
	{
		if (Target.expired())
			return Coordinates();

		std::shared_ptr<Transform> transform = GetComponent<Transform>();

		if (transform == nullptr)
			return Coordinates();

		std::shared_ptr<Transform> terrainTransform = Target.lock()->GetComponent<Transform>();

		if (terrainTransform == nullptr)
			return Coordinates();

		Vector3 cell = terrainTransform->GetWorldTransformationInverse() * transform->GetWorldPosition();

		return Coordinates(int(std::floorf(cell.X / 16)) + chunk.X, int(std::floorf(cell.Y / 16)) + chunk.Y, int(std::floorf(cell.Z / 16)) + chunk.Z);
	}

	Enum::ChunkStatus ChunkLoader::GetChunkStatus(const Coordinates& chunk) const
	{
		if (Target.expired())
			return Enum::ChunkStatus::Unloaded;

		std::shared_ptr<Transform> transform = GetComponent<Transform>();

		if (transform == nullptr)
			return Enum::ChunkStatus::Unloaded;

		std::shared_ptr<Transform> terrainTransform = Target.lock()->GetComponent<Transform>();

		if (terrainTransform == nullptr)
			return Enum::ChunkStatus::Unloaded;

		std::shared_ptr<TerrainTypes::Status> status = GetReference(terrainTransform->GetWorldTransformationInverse() * transform->GetWorldPosition(), chunk);

		if (status == nullptr)
			return Enum::ChunkStatus::Unloaded;
		else
			return status->State;
	}

	Enum::ChunkStatus ChunkLoader::GetLocalChunkStatus(const Coordinates& chunk) const
	{
		Coordinates size(2 * Range.X + 1, 2 * Range.Y + 1, 2 * Range.Z + 1);
		int index = size.Y * size.Z * (chunk.X + Range.X) + size.Z * (chunk.Y + Range.Y) + (chunk.Z + Range.Z);
		
		if (CellReferences[index] == nullptr)
			return Enum::ChunkStatus::Unloaded;

		return CellReferences[index]->State;
	}

	std::shared_ptr<TerrainTypes::Status> ChunkLoader::GetReference(const Coordinates& cell, Coordinates chunk) const
	{
		chunk.X += cell.X / 16;
		chunk.Y += cell.Y / 16;
		chunk.Z += cell.Z / 16;

		Coordinates size(2 * Range.X + 1, 2 * Range.Y + 1, 2 * Range.Z + 1);

		if (chunk.X < 0 || chunk.X >= size.X || chunk.Y < 0 || chunk.Y >= size.Y || chunk.Z < 0 || chunk.Z >= size.Z)
			return nullptr;

		return CellReferences[size.Y * size.Z * chunk.X + size.Z * chunk.Y + chunk.Z];
	}

	int square(int x)
	{
		return x * x;
	}

	void ChunkLoader::RefreshReferences()
	{
		Coordinates size(2 * Range.X + 1, 2 * Range.Y + 1, 2 * Range.Z + 1);

		for (int x = -Range.X; x <= Range.X; ++x)
		{
			for (int y = -Range.Y; y <= Range.Y; ++y)
			{
				for (int z = -Range.Z; z <= Range.Z; ++z)
				{
					int index = size.Y * size.Z * (x + Range.X) + size.Z * (y + Range.Y) + (z + Range.Z);
					bool canLoad = true;

					if (Shape == Enum::ChunkLoaderShape::Sphere)
						canLoad = x * x + y * y + z * z <= square(std::min(std::min(Range.X, Range.Y), Range.Z));
					else if (Shape == Enum::ChunkLoaderShape::CylinderX)
						canLoad = y * y + z * z <= square(std::min(Range.Y, Range.Z));
					else if (Shape == Enum::ChunkLoaderShape::CylinderY)
						canLoad = x * x + z * z <= square(std::min(Range.X, Range.Z));
					else if (Shape == Enum::ChunkLoaderShape::CylinderZ)
						canLoad = x * x + y * y <= square(std::min(Range.Y, Range.Y));

					if (!canLoad)
						CellReferences[index] = nullptr;
					else
					{
						Coordinates coords = Coordinates(Position.X + x, Position.Y + y, Position.Z + z);

						auto target = Target.lock();
						std::shared_ptr<Chunk> chunk = target->GetChunk(coords);

						if (chunk == nullptr && target->CanLoadChunk(coords))
							chunk = target->LoadChunk(coords)->Cast<Chunk>();

						if (chunk != nullptr)
							CellReferences[index] = chunk->GetStatus();
						else
							CellReferences[index] = nullptr;
					}
				}
			}
		}
	}
}