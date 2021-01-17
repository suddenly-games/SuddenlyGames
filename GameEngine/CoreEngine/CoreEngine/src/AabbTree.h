#pragma once

#include <functional>

#include "Vector3.h"
#include "Aabb.h"
#include "IdentifierHeap.h"
#include "Frustum.h"
#include "Ray.h"

class AabbTree
{
public:
	AabbTree();
	~AabbTree();

	class Node
	{
	public:
		Node* GetParent();
		const Node* GetParent() const;
		Node* GetLeft();
		const Node* GetLeft() const;
		Node* GetRight();
		const Node* GetRight() const;

		Aabb Box;
		void* ClientData = nullptr;
		int Height = 0;
		bool IsLeaf = false;
		int LastSide = 0;

		int GetID() const { return ID; }

		template <typename T>
		T* GetData() const { return reinterpret_cast<T*>(ClientData); }

		Node(AabbTree* tree, const Aabb& box, void* data, bool isLeaf = false);
		~Node();

	private:
		int ID = -1;
		int Parent = -1;
		int Left = -1;
		int Right = -1;
		AabbTree* ParentTree;

		friend class AabbTree;
	};

	typedef std::function<void(const Node* node)> Callback;
	typedef std::function<void(const Node* node, float t)> RayCallback;
	typedef std::function<void(const Node* node1, const Node* node2)> PairCallback;

	void Clear();

	int Insert(void* data, const Aabb& box);
	void Update(int nodeID, const Aabb& box);
	void ChangeData(int nodeID, void* data);
	void Remove(int nodeID);

	void PairQuery(const PairCallback& callback) const;
	void PairQuery(const AabbTree& other, const PairCallback& callback) const;
	void CastRay(const Ray& ray, const RayCallback& callback) const;
	void CastFrustum(const Frustum& frustum, const Callback& callback) const;
	void CastAabb(const Aabb& box, const Callback& callback) const;
	void VisitAll(const Callback& callback) const;

	const Node* GetRoot() const;
	const Node* GetNode(int id) const;

	// A fattening factor to use for insertion to prevent jitter from causing updates
	static const float FatteningFactor;

	// Add your implementation here
private:
	typedef IDHeap<Node> NodeHeap;

	int Root = -1;
	NodeHeap Nodes;

	Node* GetRoot();
	Node* GetNode(int id);
	Aabb GetPairBounds(const Node* node1, const Node* node2) const;
	Aabb GetPairBounds(const Aabb& first, const Aabb& second) const;
	void PushDown(Node* node, Node* parent, bool fromLeft);
	float SurfaceArea(const Aabb& box) const;
	float Volume(const Aabb& box) const;
	void RecomputeHeight(Node* node);
	void Remove(Node* node);
	Aabb Fatten(const Aabb& box);
	void Rotate(Node* node, int* connection);
	bool NodesTouch(const Node* node1, const Node* node2) const;

	void CastRay(const Ray& ray, const RayCallback& callback, const Node* node) const;
	void CastFrustum(const Frustum& frustum, const Callback& callback, const Node* node) const;
	void CastAabb(const Aabb& box, const Callback& callback, const Node* node) const;
	void PushAllChildren(const Callback& callback, const Node* node) const;

	void Query(const PairCallback& callback, const Node* node) const;
	void Query(const PairCallback& callback, const Node* node1, const Node* node2) const;
};