#pragma once

#include <limits>

#include "Plane.h"
#include "Aabb.h"

template <int Height>
class OcclusionTree : public OcclusionTree<0>
{
public:
	OcclusionTree<Height - 1> Quadrants[2][2];

	void Reset()
	{
		ResetBase();

		Quadrants[0][0].Reset();
		Quadrants[1][0].Reset();
		Quadrants[0][1].Reset();
		Quadrants[1][1].Reset();
	}

	OcclusionTree(const Aabb& boundingBox) : OcclusionTree<0>(boundingBox), Quadrants{
		{
			OcclusionTree<Height - 1>(boundingBox.GetSubRegion(Enum::Axis::MinX | Enum::Axis::MinY)),
			OcclusionTree<Height - 1>(boundingBox.GetSubRegion(Enum::Axis::MaxX | Enum::Axis::MinY))
		},
		{
			OcclusionTree<Height - 1>(boundingBox.GetSubRegion(Enum::Axis::MinX | Enum::Axis::MaxY)),
			OcclusionTree<Height - 1>(boundingBox.GetSubRegion(Enum::Axis::MaxX | Enum::Axis::MaxY))
		}
	}
	{
	}

private:

};

template <>
class OcclusionTree<0>
{
public:
	Aabb Box;

	Plane OcclusionPlane;

	virtual void Reset() { ResetBase(); }

	OcclusionTree(const Aabb& boundingBox) : Box(boundingBox) {}

protected:
	void ResetBase()
	{
		OcclusionPlane.Set(Vector3(0, 0, std::numeric_limits<float>::max()), Vector3(0, 0, 1));
	}
};

class OcclusionShape
{
public:
	virtual bool Occludes(const OcclusionTree<0>& tree) = 0;
	virtual bool OccludedBy(const OcclusionTree<0>& tree) = 0;
	virtual bool Contains(const OcclusionTree<0>& tree) = 0;
	virtual bool ContainedBy(const OcclusionTree<0>& tree) = 0;
	virtual Plane GetOcclusionPlane(const OcclusionTree<0>& tree) = 0;
};