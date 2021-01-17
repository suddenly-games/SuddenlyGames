#include "ChunkTree.h"

#include <algorithm>

#include "Chunk.h"
#include "Camera.h"

namespace Engine
{
	std::shared_ptr<Chunk> ChunkColumn::CreateChunk(const Coordinates& chunk)
	{
		if (GridScale == 0)
		{
			auto centerChunk = Engine::Create<Chunk>();

			CenterChunk = centerChunk;

			return centerChunk;
		}

		int y = (chunk.Y - Position.Y + 7 * GridScale + GridScale / 2) / GridScale;

		ChunkColumn*& child = Children[y];

		if (child == nullptr)
		{
			child = reinterpret_cast<PageAllocator<sizeof(ChunkColumn)>*>(Allocator)->Create<ChunkColumn>(Coordinates(Position.X, Position.Y + (y - 7) * GridScale, Position.Z), GridScale / 15, Allocator);

			++ActiveChildren;
		}

		return child->CreateChunk(chunk);
	}

	bool ChunkColumn::RemoveChunk(const Coordinates& chunk)
	{
		if (GridScale == 0)
		{
			CenterChunk.lock()->GetParent()->Cast<Object>()->SetParent(nullptr);

			return true;
		}

		int offset = 15 * GridScale / 2;

		if (chunk.Y < Position.Y - offset || chunk.Y > Position.Y + offset)
			return false;

		ChunkColumn*& child = Children[(chunk.Y - Position.Y + 7 * GridScale + GridScale / 2) / GridScale];

		if (child != nullptr)
		{
			bool cleanUp = child->RemoveChunk(chunk);

			if (cleanUp)
			{
				reinterpret_cast<PageAllocator<sizeof(ChunkColumn)>*>(Allocator)->Destroy<ChunkColumn>(child);

				child = nullptr;

				--ActiveChildren;
			}
		}

		return ActiveChildren == 0;
	}

	std::shared_ptr<Chunk> ChunkColumn::GetChunk(const Coordinates& chunk) const
	{
		if (GridScale == 0)
			return CenterChunk.lock();

		int offset = 15 * GridScale / 2;

		if (chunk.Y < Position.Y - offset || chunk.Y > Position.Y + offset)
			return nullptr;

		ChunkColumn* child = Children[(chunk.Y - Position.Y + 7 * GridScale + GridScale / 2) / GridScale];

		if (child != nullptr)
			return child->GetChunk(chunk);

		return nullptr;
	}

	Aabb ChunkColumn::GetBoundingBox() const
	{
		int minOffset = 0;
		int maxOffset = 16;

		if (GridScale != 0)
		{
			minOffset = 7 * GridScale * 16;
			maxOffset = 8 * GridScale * 16;
		}

		return Aabb(
			Vector3(float(Position.X * 16), float(Position.Y * 16 - minOffset), float(Position.Z * 16)),
			Vector3(float(Position.X * 16 + 16), float(Position.Y * 16 + maxOffset), float(Position.Z * 16 + 16))
		);
	}

	void ChunkColumn::Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera, const Matrix3& transformation)
	{
		if (GridScale == 0)
		{
			if (!CenterChunk.expired())
				CenterChunk.lock()->Draw(drawLiquid, targetCamera);
		}
		else
			for (int y = 0; y < 15; ++y)
				if (Children[y] != nullptr && targetCamera->GetFrustum().Intersects(Children[y]->GetBoundingBox().Transform(transformation)) != Enum::IntersectionType::Outside)
					Children[y]->Draw(drawLiquid, targetCamera, transformation);
	}

	ChunkColumn::ChunkColumn(const Coordinates& position, int gridScale, BaseAllocator* allocator) : Position(position), GridScale(gridScale), Allocator(allocator)
	{

	}

	ChunkColumn::~ChunkColumn()
	{
		if (GridScale == 0)
		{
			auto centerChunk = CenterChunk.lock();

			if (centerChunk != nullptr && centerChunk->GetParent() != nullptr)
				centerChunk->GetParent()->Cast<Object>()->SetParent(nullptr);
		}
		else
		{
			for (int y = 0; y < 15; ++y)
			{
				if (Children[y] != nullptr)
				{
					reinterpret_cast<PageAllocator<sizeof(ChunkColumn)>*>(Allocator)->Destroy<ChunkColumn>(Children[y]);

					Children[y] = nullptr;
				}
			}
		}
	}

	bool ChunkColumn::IsLeaf() const
	{
		return GridScale == 0;
	}

	int ChunkColumn::GetChildIndex(Coordinates chunk) const
	{
		int offset = 15 * GridScale / 2;

		if (chunk.Y < Position.Y - offset || chunk.Y > Position.Y + offset)
			return -1;

		return (chunk.Y - Position.Y + 7 * GridScale + GridScale / 2) / GridScale;
	}

	const ChunkColumn* ChunkColumn::GetChild(int index) const
	{
		return Children[index];
	}

	std::shared_ptr<Chunk> ChunkColumn::GetChunk() const
	{
		return CenterChunk.lock();
	}

	ChunkColumn* ChunkColumn::Expand(const Coordinates& chunk)
	{
		int offset = 15 * GridScale / 2;

		if (chunk.Y < Position.Y - offset || chunk.Y > Position.Y + offset)
		{
			ChunkColumn* parent = reinterpret_cast<PageAllocator<sizeof(ChunkColumn)>*>(Allocator)->Create<ChunkColumn>(Position, GridScale * 15, Allocator);

			parent->Children[7] = this;
			++parent->ActiveChildren;

			return parent->Expand(chunk);
		}

		return this;
	}

	void ChunkColumn::ExpandExtents(const Coordinates& chunk, int& gridScale)
	{
		while (chunk.Y < -15 * gridScale / 2 || chunk.Y > 15 * gridScale / 2)
			gridScale *= 15;
	}

	std::shared_ptr<Chunk> ChunkTree::CreateChunk(const Coordinates& chunk)
	{
		if (GridScale == 0)
		{
			if (CenterColumn == nullptr)
			{
				LargestChildScale = 1;

				ChunkColumn::ExpandExtents(chunk, LargestChildScale);

				CenterColumn = ColumnAllocator->Create<ChunkColumn>(Coordinates(PositionX, 0, PositionZ), LargestChildScale, ColumnAllocator);
			}

			return CenterColumn->CreateChunk(chunk);
		}

		int x = (chunk.X - PositionX + 7 * GridScale + GridScale / 2) / GridScale;
		int z = (chunk.Z - PositionZ + 7 * GridScale + GridScale / 2) / GridScale;

		ChunkTree*& child = Children[x][z];

		if (child == nullptr)
		{
			child = reinterpret_cast<PageAllocator<sizeof(ChunkTree)>*>(Allocator)->Create<ChunkTree>(PositionX + (x - 7) * GridScale, PositionZ + (z - 7) * GridScale, GridScale / 15, Allocator, ColumnAllocator);

			++ActiveChildren;
		}

		std::shared_ptr<Chunk> newChunk = child->CreateChunk(chunk);

		LargestChildScale = std::max(LargestChildScale, child->LargestChildScale);
		
		return newChunk;
	}

	bool ChunkTree::RemoveChunk(const Coordinates& chunk)
	{
		if (GridScale == 0)
		{
			bool cleanUp = CenterColumn->RemoveChunk(chunk);

			return cleanUp;
		}

		int offset = 15 * GridScale / 2;

		if (chunk.X < PositionX - offset || chunk.X > PositionX + offset || chunk.Z < PositionZ - offset || chunk.Z > PositionZ + offset)
			return false;

		ChunkTree*& child = Children[(chunk.X - PositionX + 7 * GridScale + GridScale / 2) / GridScale][(chunk.Z - PositionZ + 7 * GridScale + GridScale / 2) / GridScale];

		if (child != nullptr)
		{
			bool cleanUp = child->RemoveChunk(chunk);

			if (cleanUp)
			{
				reinterpret_cast<PageAllocator<sizeof(ChunkTree)>*>(Allocator)->Destroy<ChunkTree>(child);

				child = nullptr;

				--ActiveChildren;
			}
		}

		LargestChildScale = 0;

		for (int x = 0; x < 15; ++x)
			for (int z = 0; z < 15; ++z)
				if (Children[x][z] != nullptr)
					LargestChildScale = std::max(LargestChildScale, Children[x][z]->LargestChildScale);

		return ActiveChildren == 0;
	}

	std::shared_ptr<Chunk> ChunkTree::GetChunk(const Coordinates& chunk) const
	{
		if (GridScale == 0)
			return CenterColumn->GetChunk(chunk);

		int offset = 15 * GridScale / 2;

		if (chunk.X < PositionX - offset || chunk.X > PositionX + offset || chunk.Z < PositionZ - offset || chunk.Z > PositionZ + offset)
			return nullptr;

		ChunkTree* child = Children[(chunk.X - PositionX + 7 * GridScale + GridScale / 2) / GridScale][(chunk.Z - PositionZ + 7 * GridScale + GridScale / 2) / GridScale];

		if (child != nullptr)
			return child->GetChunk(chunk);

		return nullptr;
	}

	Aabb ChunkTree::GetBoundingBox() const
	{
		int minOffset = 0;
		int maxOffset = 16;

		if (GridScale != 0)
		{
			minOffset = 7 * GridScale * 16;
			maxOffset = 8 * GridScale * 16;
		}

		int minHeightOffset = 0;
		int maxHeightOffset = 16;

		if (LargestChildScale != 0)
		{
			minHeightOffset = 7 * LargestChildScale * 16;
			maxHeightOffset = 8 * LargestChildScale * 16;
		}

		return Aabb(
			Vector3(float(PositionX * 16 - minOffset), float(-minHeightOffset), float(PositionZ * 16 - minOffset)),
			Vector3(float(PositionX * 16 + maxOffset), float(maxHeightOffset), float(PositionZ * 16 + maxOffset))
		);
	}

	void ChunkTree::Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera, const Matrix3& transformation) const
	{
		if (GridScale == 0)
			CenterColumn->Draw(drawLiquid, targetCamera, transformation);
		else
			for (int x = 0; x < 15; ++x)
				for (int z = 0; z < 15; ++z)
					if (Children[x][z] != nullptr && targetCamera->GetFrustum().Intersects(Children[x][z]->GetBoundingBox().Transform(transformation)) != Enum::IntersectionType::Outside)
						Children[x][z]->Draw(drawLiquid, targetCamera, transformation);
	}

	ChunkTree::ChunkTree(int positionX, int positionZ, int gridScale, BaseAllocator* allocator, PageAllocator<sizeof(ChunkColumn)>* columnAllocator) : PositionX(positionX), PositionZ(positionZ), GridScale(gridScale), Allocator(allocator), ColumnAllocator(columnAllocator)
	{

	}

	ChunkTree::~ChunkTree()
	{
		if (GridScale == 0)
		{
			if (CenterColumn != nullptr)
				ColumnAllocator->Destroy<ChunkColumn>(CenterColumn);

			CenterColumn = nullptr;
		}
		else
		{
			for (int x = 0; x < 15; ++x)
			{
				for (int z = 0; z < 15; ++z)
				{
					if (Children[x][z] != nullptr)
					{
						reinterpret_cast<PageAllocator<sizeof(ChunkTree)>*>(Allocator)->Destroy<ChunkTree>(Children[x][z]);

						Children[x][z] = nullptr;
					}
				}
			}
		}
	}

	bool ChunkTree::IsLeaf() const
	{
		return GridScale == 0;
	}

	Coordinates ChunkTree::GetChildIndex(Coordinates chunk) const
	{
		int offset = 15 * GridScale / 2;

		if (chunk.X < PositionX - offset || chunk.X > PositionX + offset || chunk.Z < PositionZ - offset || chunk.Z > PositionZ + offset)
			return Coordinates(-1, -1, -1);

		return Coordinates((chunk.X - PositionX + 7 * GridScale + GridScale / 2) / GridScale, (chunk.Z - PositionZ + 7 * GridScale + GridScale / 2) / GridScale);
	}

	const ChunkTree* ChunkTree::GetChild(Coordinates index) const
	{
		return Children[index.X][index.Z];
	}

	const ChunkColumn* ChunkTree::GetColumn() const
	{
		return CenterColumn;
	}

	ChunkTree* ChunkTree::Expand(const Coordinates& chunk)
	{
		int offset = 15 * GridScale / 2;

		if (chunk.X < PositionX - offset || chunk.X > PositionX + offset || chunk.Z < PositionZ - offset || chunk.Z > PositionZ + offset)
		{
			ChunkTree* parent = reinterpret_cast<PageAllocator<sizeof(ChunkTree)>*>(Allocator)->Create<ChunkTree>(0, 0, GridScale * 15, Allocator, ColumnAllocator);

			parent->Children[7][7] = this;
			++parent->ActiveChildren;

			return parent->Expand(chunk);
		}

		return this;
	}

	void ChunkTree::ExpandExtents(const Coordinates& chunk, int& gridScale)
	{
		while (chunk.X < -15 * gridScale / 2 || chunk.X > 15 * gridScale / 2 || chunk.Z < -15 * gridScale / 2 || chunk.Z > 15 * gridScale / 2)
			gridScale *= 15;
	}
}