#pragma once

#include "TerrainMaterial.h"
#include "Voxel.h"

namespace Engine
{
	class CellData
	{
	public:
		float Occupancy = 0;
		std::shared_ptr<TerrainMaterial> MaterialType;
		std::shared_ptr<Voxel> VoxelData;

		CellData() {}
		CellData(float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& voxel) : Occupancy(occupancy), MaterialType(material), VoxelData(voxel) {}

		Base_Class;

		Reflected_Type(CellData);
	};

	//Class_Inherits(CellData, CoreType);

	Define_Value_Type(CellData);
}