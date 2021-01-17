#include "Terrain2DCollider.h"

#include "ColliderData.h"

namespace Engine
{
	void Terrain2DCollider::PairQuery(const AabbTree::Node* head, const TerrainPairCallback& collisionCallback, const ColliderCallback& lookupCallback)
	{
		Physics::ColliderData collider;


	}

	void Terrain2DCollider::Visit(const AabbTree::Node* node, const ChunkTree* chunks, Physics::ColliderData& collider)
	{

	}

	void Terrain2DCollider::Visit(const AabbTree::Node* node, const ChunkColumn* chunks, Physics::ColliderData& collider)
	{

	}

	void Terrain2DCollider::Visit(const AabbTree::Node* node, const std::shared_ptr<Chunk>& chunk, Physics::ColliderData& collider)
	{

	}
}