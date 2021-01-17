#include "AabbTree.h"

const float AabbTree::FatteningFactor = 1.01f;

typename AabbTree::Node* AabbTree::Node::GetParent()
{
	if (Parent == -1)
		return nullptr;

	return ParentTree->GetNode(Parent);
}

typename const AabbTree::Node* AabbTree::Node::GetParent() const
{
	if (Parent == -1)
		return nullptr;

	return ParentTree->GetNode(Parent);
}

typename AabbTree::Node* AabbTree::Node::GetLeft()
{
	if (Left == -1)
		return nullptr;

	return ParentTree->GetNode(Left);
}

typename const AabbTree::Node* AabbTree::Node::GetLeft() const
{
	if (Left == -1)
		return nullptr;

	return ParentTree->GetNode(Left);
}

typename AabbTree::Node* AabbTree::Node::GetRight()
{
	if (Right == -1)
		return nullptr;

	return ParentTree->GetNode(Right);
}

typename const AabbTree::Node* AabbTree::Node::GetRight() const
{
	if (Right == -1)
		return nullptr;

	return ParentTree->GetNode(Right);
}

AabbTree::Node::Node(AabbTree* tree, const Aabb& box, void* data, bool isLeaf) : ParentTree(tree), Box(box), ClientData(data), IsLeaf(isLeaf)
{

}

AabbTree::Node::~Node()
{
	if (Left != -1)
		ParentTree->Nodes.Release(Left);

	if (Right != -1)
		ParentTree->Nodes.Release(Right);
}

AabbTree::AabbTree()
{

}

AabbTree::~AabbTree()
{
	Clear();
}

void AabbTree::Clear()
{
	if (Root != -1)
		Nodes.Release(Root);

	Root = -1;
}

int AabbTree::Insert(void* data, const Aabb& box)
{
	int nodeID = Nodes.RequestID(this, Fatten(box), data, true);

	Node* node = GetNode(nodeID);

	node->ID = nodeID;

	if (Root == -1)
		Root = nodeID;
	else
		PushDown(node, GetRoot(), false);

	return nodeID;
}

void AabbTree::Update(int nodeID, const Aabb& box)
{
	Node* node = GetNode(nodeID);

	if (node->Box.ContainsPoint(box.Min) && node->Box.ContainsPoint(box.Max))
	{
		node->Box = box;

		return;
	}

	node->Box = Fatten(box);

	if (nodeID != Root)
	{
		Remove(node);

		PushDown(node, GetRoot(), false);
	}
}

void AabbTree::ChangeData(int nodeID, void* data)
{
	GetNode(nodeID)->ClientData = data;
}

void AabbTree::Remove(int nodeID)
{
	Remove(GetNode(nodeID));

	Nodes.Release(nodeID);
}

void AabbTree::PairQuery(const PairCallback& callback) const
{
	if (Root != -1)
		Query(callback, GetRoot());
}

void AabbTree::PairQuery(const AabbTree& other, const PairCallback& callback) const
{
	if (Root != -1 && other.Root != -1)
		Query(callback, GetRoot(), other.GetRoot());
}

void AabbTree::CastRay(const Ray& ray, const RayCallback& callback) const
{
	if (Root != -1)
		CastRay(ray, callback, GetRoot());
}

void AabbTree::CastFrustum(const Frustum& frustum, const Callback& callback) const
{
	if (Root != -1)
		CastFrustum(frustum, callback, GetRoot());
}

void AabbTree::CastAabb(const Aabb& box, const Callback& callback) const
{
	if (Root != -1)
		CastAabb(box, callback, GetRoot());
}

void AabbTree::VisitAll(const Callback& callback) const
{
	if (Root != -1)
		PushAllChildren(callback, GetRoot());
}

typename AabbTree::Node* AabbTree::GetRoot()
{
	return GetNode(Root);
}

typename AabbTree::Node* AabbTree::GetNode(int id)
{
	if (id < 0 || id >= int(Nodes.Size()))
		return nullptr;

	return &Nodes.GetNode(id).GetData();
}

typename const AabbTree::Node* AabbTree::GetRoot() const
{
	return GetNode(Root);
}

typename const AabbTree::Node* AabbTree::GetNode(int id) const
{
	if (id < 0 || id >= int(Nodes.Size()))
		return nullptr;

	return &Nodes.GetNode(id).GetData();
}

Aabb AabbTree::GetPairBounds(const Node* node1, const Node* node2) const
{
	if (node1 == nullptr)
		return node2->Box;
	else if (node2 == nullptr)
		return node1->Box;

	return GetPairBounds(node1->Box, node2->Box);
}

Aabb AabbTree::GetPairBounds(const Aabb& first, const Aabb& second) const
{
	Vector3 minCorner = first.Min;
	Vector3 maxCorner = first.Max;

	if (second.Min.X < minCorner.X)
		minCorner.X = second.Min.X;

	if (second.Min.Y < minCorner.Y)
		minCorner.Y = second.Min.Y;

	if (second.Min.Z< minCorner.Z)
		minCorner.Z = second.Min.Z;

	if (second.Max.X > maxCorner.X)
		maxCorner.X = second.Max.X;

	if (second.Max.Y > maxCorner.Y)
		maxCorner.Y = second.Max.Y;

	if (second.Max.Z > maxCorner.Z)
		maxCorner.Z = second.Max.Z;

	return Aabb(minCorner, maxCorner);
}

void AabbTree::PushDown(Node* node, Node* parent, bool fromLeft)
{
	int* connection = &Root;

	if (parent->Parent != -1)
	{
		if (fromLeft)
			connection = &parent->GetParent()->Left;
		else
			connection = &parent->GetParent()->Right;
	}

	int nodeID = node->ID;
	int parentID = parent->ID;

	if (parent->IsLeaf)
	{
		int bridgeID = Nodes.RequestID(this, GetPairBounds(parent, node), nullptr);
		Node* bridge = GetNode(bridgeID);

		node = GetNode(nodeID);
		parent = GetNode(parentID);

		if (parent->Parent != -1)
		{
			if (fromLeft)
				connection = &parent->GetParent()->Left;
			else
				connection = &parent->GetParent()->Right;
		}

		bridge->ID = bridgeID;
		bridge->Left = parent->ID;
		bridge->Right = node->ID;
		bridge->Height = 1;

		bridge->Parent = parent->Parent;
		parent->Parent = bridge->ID;
		node->Parent = bridge->ID;

		*connection = bridge->ID;

		if (bridge->Parent != -1)
			RecomputeHeight(bridge->GetParent());

		return;
	}

	Aabb newBoundsLeft = GetPairBounds(parent->GetLeft(), node);
	Aabb newBoundsRight = GetPairBounds(parent->GetRight(), node);

	float surfaceAreaLeft = SurfaceArea(parent->GetLeft()->Box);
	float surfaceAreaRight = SurfaceArea(parent->GetRight()->Box);
	float newSurfaceAreaLeft = SurfaceArea(newBoundsLeft);
	float newSurfaceAreaRight = SurfaceArea(newBoundsRight);

	float changeLeft = newSurfaceAreaLeft - surfaceAreaLeft;
	float changeRight = newSurfaceAreaRight - surfaceAreaRight;

	if (changeRight <= changeLeft)
		PushDown(node, parent->GetRight(), false);
	else
		PushDown(node, parent->GetLeft(), true);
}

float AabbTree::SurfaceArea(const Aabb& box) const
{
	Vector3 diff = box.Max - box.Min;

	return 2 * diff.X * diff.Y + 2 * diff.Y * diff.Z + 2 * diff.X * diff.Z;
}

float AabbTree::Volume(const Aabb& box) const
{
	Vector3 diff = box.Max - box.Min;

	return diff.X * diff.Y * diff.Z;
}

void AabbTree::RecomputeHeight(Node* node)
{

	if (node == nullptr)
		return;

	int leftHeight = node->GetLeft()->Height;
	int rightHeight = node->GetRight()->Height;

	node->Box = GetPairBounds(node->GetLeft()->Box, node->GetRight()->Box);

	if (leftHeight - rightHeight > 1 || rightHeight - leftHeight > 1)
	{
		int* connection = &Root;

		if (node->Parent != -1)
		{
			if (node->GetParent()->Left == node->ID)
				connection = &node->GetParent()->Left;
			else
				connection = &node->GetParent()->Right;
		}

		Rotate(node, connection);
	}
	else
	{
		if (leftHeight > rightHeight)
			node->Height = leftHeight + 1;
		else
			node->Height = rightHeight + 1;
	}

	if (node->Parent != -1)
		RecomputeHeight(node->GetParent());
}

void AabbTree::Remove(Node* node)
{
	if (node->Parent == -1)
	{
		Nodes.Release(node->ID);

		Root = -1;
	}
	else if (node->GetParent()->Parent == -1)
	{
		if (node->GetParent()->GetLeft() == node)
			Root = node->GetParent()->Right;
		else
			Root = node->GetParent()->Left;

		GetRoot()->Parent = -1;

		node->GetParent()->Left = -1;
		node->GetParent()->Right = -1;

		Nodes.Release(node->Parent);

		node->Parent = -1;
	}
	else
	{
		Node* otherNode = node->GetParent()->GetLeft();

		if (otherNode == node)
			otherNode = node->GetParent()->GetRight();

		int* link = &node->GetParent()->GetParent()->Left;

		if (*link != node->Parent)
			link = &node->GetParent()->GetParent()->Right;

		*link = otherNode->ID;

		otherNode->Parent = node->GetParent()->Parent;

		node->GetParent()->Left = -1;
		node->GetParent()->Right = -1;

		Nodes.Release(node->Parent);

		node->Parent = -1;

		RecomputeHeight(otherNode->GetParent());
	}
}

Aabb AabbTree::Fatten(const Aabb& box)
{
	Vector3 diff = box.Max - box.Min;

	diff = 0.5f * (FatteningFactor * diff - box.Max + box.Min);

	return Aabb(box.Min - diff, box.Max + diff);
}

void AabbTree::Rotate(Node* node, int* connection)
{
	int* pivot = &node->Left;

	if (node->GetRight()->Height > node->GetLeft()->Height)
		pivot = &node->Right;

	int* largeChild = &GetNode(*pivot)->Right;

	if (GetNode(*pivot)->GetLeft()->Height < GetNode(*largeChild)->Height)
		largeChild = &GetNode(*pivot)->Left;

	GetNode(*pivot)->Parent = node->Parent;
	*connection = *pivot;
	GetNode(*largeChild)->Parent = node->ID;
	*pivot = *largeChild;
	*largeChild = node->ID;
	node->Parent = *connection;

	node->Height = node->GetLeft()->Height + 1;
	node->Box = GetPairBounds(node->GetLeft(), node->GetRight());

	if (node->GetRight()->Height > node->GetLeft()->Height)
		node->Height = node->GetRight()->Height + 1;

	GetNode(*connection)->Height = GetNode(*connection)->GetRight()->Height + 1;

	if (GetNode(*connection)->GetLeft()->Height > GetNode(*connection)->GetRight()->Height)
		GetNode(*connection)->Height = GetNode(*connection)->GetLeft()->Height + 1;

	GetNode(*connection)->Box = GetPairBounds(GetNode(*connection)->GetLeft(), GetNode(*connection)->GetRight());
}

bool AabbTree::NodesTouch(const Node* node1, const Node* node2) const
{
	return node1->Box.Intersects(node2->Box);
}

void AabbTree::CastRay(const Ray& ray, const RayCallback& callback, const Node* node) const
{
	float t = -1;

	if (ray.Intersects(node->Box, t, 1e-5f))
	{
		if (node->IsLeaf)
		{
			callback(node, t);

			return;
		}

		if (node->Left != -1)
			CastRay(ray, callback, node->GetLeft());

		if (node->Right != -1)
			CastRay(ray, callback, node->GetRight());
	}
}

void AabbTree::CastFrustum(const Frustum& frustum, const Callback& callback, const Node* node) const
{
	Enum::IntersectionType intersection = frustum.Intersects(node->Box);

	if (intersection == Enum::IntersectionType::Inside)
		PushAllChildren(callback, node);
	else if (intersection != Enum::IntersectionType::Outside)
	{
		if (node->IsLeaf)
		{
			callback(node);

			return;
		}

		if (node->Left != -1)
			CastFrustum(frustum, callback, node->GetLeft());

		if (node->Right != - 1)
			CastFrustum(frustum, callback, node->GetRight());
	}
}

void AabbTree::CastAabb(const Aabb& box, const Callback& callback, const Node* node) const
{
	if (node->Box.Intersects(box))
	{
		if (node->IsLeaf)
			callback(node);
		else if (box.ContainsPoint(node->Box.Min) && box.ContainsPoint(node->Box.Max))
		{
			if (node->Left != -1)
				PushAllChildren(callback, node->GetLeft());

			if (node->Right != -1)
				PushAllChildren(callback, node->GetRight());
		}
		else
		{
			if (node->Left != -1)
				CastAabb(box, callback, node->GetLeft());

			if (node->Right != -1)
				CastAabb(box, callback, node->GetRight());
		}
	}
}

void AabbTree::PushAllChildren(const Callback& callback, const Node* node) const
{
	if (node->IsLeaf)
		callback(node);
	else
	{
		if (node->Left != -1)
			PushAllChildren(callback, node->GetLeft());

		if (node->Right != -1)
			PushAllChildren(callback, node->GetRight());
	}
}

void AabbTree::Query(const PairCallback& callback, const Node* node) const
{
	if (node->IsLeaf)
		return;

	Query(callback, node->GetLeft());
	Query(callback, node->GetRight());
	Query(callback, node->GetLeft(), node->GetRight());
}

void AabbTree::Query(const PairCallback& callback, const Node* node1, const Node* node2) const
{
	if (!NodesTouch(node1, node2))
		return;

	if (node1->IsLeaf && node2->IsLeaf)
	{
		callback(node1, node2);

		return;
	}
	else if (node1->IsLeaf)
	{
		Query(callback, node1, node2->GetLeft());
		Query(callback, node1, node2->GetRight());
	}
	else if (node2->IsLeaf)
	{
		Query(callback, node1->GetLeft(), node2);
		Query(callback, node1->GetRight(), node2);
	}
	else
	{
		float vol1 = Volume(node1->Box);
		float vol2 = Volume(node2->Box);

		if (vol1 < vol2)
		{
			Query(callback, node1, node2->GetLeft());
			Query(callback, node1, node2->GetRight());
		}
		else
		{
			Query(callback, node1->GetLeft(), node2);
			Query(callback, node1->GetRight(), node2);
		}
	}
}
