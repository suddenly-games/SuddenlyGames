#include "ModelAsset.h"

namespace Engine
{
	Reflect_Inherited(ModelAsset, Object,
		Document_Class("");
		
		Bind_Function(GetMeshID,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
	
				Overload_Parameters();

				Bind_Parameters(GetMeshID);
			);
		);
		
		Bind_Function(IsCore,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
	
				Overload_Parameters();

				Bind_Parameters(IsCore);
			);
		);
		
		Bind_Function(GetMinimumCorner,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters();

				Bind_Parameters(GetMinimumCorner);
			);
		);
		
		Bind_Function(GetMaximumCorner,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters();

				Bind_Parameters(GetMaximumCorner);
			);
		);
		
		Bind_Function(GetCenter,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters();

				Bind_Parameters(GetCenter);
			);
		);
		
		Bind_Function(GetSize,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters();

				Bind_Parameters(GetSize);
			);
		);
		
		Bind_Function(IsLoaded,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
	
				Overload_Parameters();

				Bind_Parameters(IsLoaded);
			);
		);
		
		Bind_Function(Load,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
		
				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, path);
		
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::VertexMode>, mode, Enum::VertexMode::Seperate);
				);
		
				Bind_Parameters_No_Return(Load, path, mode);
			);
		);
		
		Bind_Function(Unload,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
		
				Overload_Parameters();
		
				Bind_Parameters_No_Return(Unload);
			);
		);
		
		Bind_Function(Release,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
		
				Overload_Parameters();
		
				Bind_Parameters_No_Return(Release);
			);
		);
	);
}