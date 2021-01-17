#include "Terrain.h"

namespace Engine
{
    Reflect_Inherited(Terrain, Object,
        Document_Class("");

        Document("");
        Archivable Class_Member(std::string, DataDirectory);

		Bind_Function(GetCell,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(CellData);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);
				);
			
				Bind_Parameters(GetCell, cell);
			);
		);

		Bind_Function(GetMaterialID,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(unsigned short);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);
				);
			
				Bind_Parameters(GetMaterialID, cell);
			);
		);

		Bind_Function(CanLoadChunk,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, chunk);
				);
			
				Bind_Parameters(CanLoadChunk, chunk);
			);
		);

		Bind_Function(GetChunkStatus,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::ChunkStatus>);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, chunk);
				);
			
				Bind_Parameters(GetChunkStatus, chunk);
			);
		);

		Bind_Function(GetChunkCoordinates,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);
				);
			
				Bind_Parameters(GetChunkCoordinates, cell);
			);
		);

		Bind_Function(GetCellCoordinates,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);
				);
			
				Bind_Parameters(GetCellCoordinates, cell);
			);
		);

		Bind_Function(SetCell,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);

					Document("");
					Function_Parameter(CellData, data);
				);
			
				Bind_Parameters_No_Return(SetCell, cell, data);
			);
		);

		Bind_Function(SetCell,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);

					Document("");
					Function_Parameter(float, occupancy);

					Document("");
					Function_Parameter(std::shared_ptr<TerrainMaterial>, material);

					Document("");
					Function_Parameter(std::shared_ptr<Voxel>, data);
				);
			
				Bind_Parameters_No_Return(SetCell, cell, occupancy, material, data);
			);
		);

		Bind_Function(SetOccupancy,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);

					Document("");
					Function_Parameter(float, occupancy);
				);
			
				Bind_Parameters_No_Return(SetOccupancy, cell, occupancy);
			);
		);

		Bind_Function(SetMaterial,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);

					Document("");
					Function_Parameter(std::shared_ptr<TerrainMaterial>, material);
				);
			
				Bind_Parameters_No_Return(SetMaterial, cell, material);
			);
		);

		Bind_Function(SetData,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, cell);

					Document("");
					Function_Parameter(std::shared_ptr<Voxel>, data);
				);
			
				Bind_Parameters_No_Return(SetData, cell, data);
			);
		);
    );
}