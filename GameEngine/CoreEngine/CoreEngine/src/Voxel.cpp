#include "Voxel.h"

namespace Engine
{
	void Voxel::SetPosition(const Coordinates& position)
	{
		PositionChanged(Position, position);

		Position = position;
	}

	Coordinates Voxel::GetPosition() const
	{
		return Position;
	}
}