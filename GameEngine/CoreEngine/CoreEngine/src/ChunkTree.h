#pragma once

#include "PageAllocator.h"
#include "Vector3.h"
#include "Coordinates.h"
#include "Aabb.h"

namespace GraphicsEngine
{
	class Camera;
}

namespace Engine
{
	class Chunk;

	class ChunkColumn
	{
	public:
		std::shared_ptr<Chunk> CreateChunk(const Coordinates& chunk);
		bool RemoveChunk(const Coordinates& chunk);
		std::shared_ptr<Chunk> GetChunk(const Coordinates& chunk) const;
		Aabb GetBoundingBox() const;
		void Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera, const Matrix3& transformation);

		ChunkColumn(const Coordinates& position, int gridScale, BaseAllocator* allocator);
		~ChunkColumn();

		bool IsLeaf() const;
		int GetChildIndex(Coordinates chunk) const;
		const ChunkColumn* GetChild(int index) const;
		std::shared_ptr<Chunk> GetChunk() const;

		ChunkColumn* Expand(const Coordinates& chunk);
		static void ExpandExtents(const Coordinates& chunk, int& gridScale);

	private:
		Coordinates Position;
		int GridScale = 1;
		int ActiveChildren = 0;

		ChunkColumn* Children[15] = {};
		BaseAllocator* Allocator = nullptr;

		std::weak_ptr<Chunk> CenterChunk;
	};

	class ChunkTree
	{
	public:
		std::shared_ptr<Chunk> CreateChunk(const Coordinates& chunk);
		bool RemoveChunk(const Coordinates& chunk);
		std::shared_ptr<Chunk> GetChunk(const Coordinates& chunk) const;
		Aabb GetBoundingBox() const;
		void Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera, const Matrix3& transformation) const;

		ChunkTree(int positionX, int positionZ, int gridScale, BaseAllocator* allocator, PageAllocator<sizeof(ChunkColumn)>* columnAllocator);
		~ChunkTree();

		bool IsLeaf() const;
		Coordinates GetChildIndex(Coordinates chunk) const;
		const ChunkTree* GetChild(Coordinates index) const;
		const ChunkColumn* GetColumn() const;

		ChunkTree* Expand(const Coordinates& chunk);
		static void ExpandExtents(const Coordinates& chunk, int& gridScale);

	private:
		int PositionX = 0;
		int PositionZ = 0;
		int GridScale = 1;
		int ActiveChildren = 0;
		int LargestChildScale = 0;

		ChunkColumn* CenterColumn = nullptr;
		ChunkTree* Children[15][15] = {};
		BaseAllocator* Allocator = nullptr;
		PageAllocator<sizeof(ChunkColumn)>* ColumnAllocator = nullptr;
	};
}