#include "CellData.h"

using Engine::CellData;

Reflect_Type(CellData,
	Document_Class(
		""
	);

	No_Reference;

	Document("");
	Read_Only Archivable Class_Member(float, Occupancy);

	Document("");
	Read_Only Archivable Class_Member(std::shared_ptr<TerrainMaterial>, MaterialType);

	Document("");
	Read_Only Archivable Class_Member(std::shared_ptr<Voxel>, VoxelData);
);