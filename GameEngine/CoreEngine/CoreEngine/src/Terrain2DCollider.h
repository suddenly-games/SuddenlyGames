#pragma once

#include "Object.h"
#include "AabbTree.h"

namespace Engine
{
	class Chunk;
	class ChunkTree;
	class ChunkColumn;

	namespace Physics
	{
		class Collider2D;
		class ColliderData;
	}

	class Terrain2DCollider : public Object
	{
	public:
		typedef std::function<void(const std::shared_ptr<Physics::Collider2D>& collider, const Physics::ColliderData& cell)> TerrainPairCallback;
		typedef std::function<std::shared_ptr<Physics::Collider2D>(void*)> ColliderCallback;

		void Initialize() {}
		void Update(float) {}

		void PairQuery(const AabbTree::Node* head, const TerrainPairCallback& collisionCallback, const ColliderCallback& lookupCallback);

	private:
		void Visit(const AabbTree::Node* node, const ChunkTree* chunks, Physics::ColliderData& collider);
		void Visit(const AabbTree::Node* node, const ChunkColumn* chunks, Physics::ColliderData& collider);
		void Visit(const AabbTree::Node* node, const std::shared_ptr<Chunk>& chunk, Physics::ColliderData& collider);
	};
}