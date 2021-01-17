#include "ChunkLoader.h"

namespace Engine
{
    Reflect_Inherited(ChunkLoader, Object,
        Document_Class("");

        Document("");
        Archivable Class_Member(std::weak_ptr<Terrain>, Target);

		Document("");
		Archivable Class_Member(LuaEnum < Enum::ChunkLoaderShape>, Shape);

		Bind_Function(GetCellCoordinates,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetCellCoordinates);
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
					Function_Parameter(Vector3, chunk);
				);
			
				Bind_Parameters(GetChunkCoordinates, chunk);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetChunkCoordinates);
			);
		);

		Bind_Function(GetRange,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetRange);
			);
		);

		Bind_Function(SetRange,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, range);
				);
			
				Bind_Parameters_No_Return(SetRange, range);
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

		Bind_Function(GetLocalChunkStatus,
		
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
			
				Bind_Parameters(GetLocalChunkStatus, chunk);
			);
		);
    );
}