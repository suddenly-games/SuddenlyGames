#pragma once

#include <memory>

#include "ChunkStatusEnum.h"

namespace TerrainTypes
{
	class VoxelData
	{
	public:
		unsigned char Occupancy = 0;
		unsigned short MaterialData = 0;
		unsigned short Data = 0xFFF;

		float GetOccupancy() const;
		void SetOccupancy(float occupancy);

		int GetDataIndex() const;
		void SetDataIndex(int index);

		VoxelData& operator=(const VoxelData& other);
	};

	struct Status
	{
		Enum::ChunkStatus State = Enum::ChunkStatus::Unloaded;

		static std::shared_ptr<Status> Unloaded();
	};
}