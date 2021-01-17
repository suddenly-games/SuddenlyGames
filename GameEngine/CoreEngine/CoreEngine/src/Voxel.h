#pragma once

#include "Object.h"
#include "Coordinates.h"

namespace Engine
{
	class Voxel : public Object
	{
	public:
		void Initialize() {}
		void Update(float delta) {}

		void SetPosition(const Coordinates& position);
		Coordinates GetPosition() const;

		virtual void Removed() {}

	protected:
		virtual void PositionChanged(const Coordinates& oldPosition, const Coordinates& newPosition) {}

	private:
		Coordinates Position;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Voxel);
	};

	//Class_Inherits(Voxel, Object);
}