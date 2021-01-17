#include "TerrainTypes.h"

namespace TerrainTypes
{
	std::shared_ptr<Status> Status::Unloaded()
	{
		static Status status;
		static std::shared_ptr<Status> state(&status, [] (void*) {});

		status.State = Enum::ChunkStatus::Unloaded;

		return state;
	}

	float VoxelData::GetOccupancy() const
	{
		if (Data & 0x8000)
			return 1;
		else
			return float(Occupancy) / 0x100;
	}

	void VoxelData::SetOccupancy(float occupancy)
	{
		if (occupancy > float(0xFF) / float(0x100))
			Data |= 0x8000;
		else
		{
			Data &= 0x7FFF;
			Occupancy = char(occupancy * float(0x100));
		}
	}

	int VoxelData::GetDataIndex() const
	{
		return Data & 0xFFF;
	}

	void VoxelData::SetDataIndex(int index)
	{
		Data &= 0x8000;
		Data |= index;
	}

	VoxelData& VoxelData::operator=(const VoxelData& other)
	{
		Occupancy = other.Occupancy;
		MaterialData = other.MaterialData;
		Data = other.Data;

		return *this;
	}
}