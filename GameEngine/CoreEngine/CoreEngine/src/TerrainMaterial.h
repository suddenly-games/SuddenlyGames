#pragma once

#include "Object.h"

namespace Engine
{
	class TerrainMaterial : public Object
	{
	public:
		void Initialize() {}
		void Update(float delta) {}

		unsigned short GetID() const { return -1; }

	private:

		Instantiable;

		Inherits_Class(Object);

		Reflected(TerrainMaterial);

	};

	//Class_Inherits(TerrainMaterial, Object);
}