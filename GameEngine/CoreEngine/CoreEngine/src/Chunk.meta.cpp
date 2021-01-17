#include "Chunk.h"

namespace Engine
{
    Reflect_Inherited(Chunk, Object,
        Document_Class("");

		Bind_Function(Save,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters();
			
				Bind_Parameters_No_Return(Save);
			);
		);

		Bind_Function(Load,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters();
			
				Bind_Parameters_No_Return(Save);
			);
		);

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

		Bind_Function(GetCoordinates,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetCoordinates);
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

		Bind_Function(GetTerrain,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Object>);
			
				Overload_Parameters();
			
				Bind_Parameters(GetTerrain);
			);
		);
    );
}