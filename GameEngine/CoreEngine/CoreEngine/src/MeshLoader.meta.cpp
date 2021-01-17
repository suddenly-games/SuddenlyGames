#include "MeshLoader.h"

#include "ModelAsset.h"

using Engine::Object;

Reflect_Inherited(MeshLoader, Object,
	Document_Class("");
	
	Bind_Function(NewAsset,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(std::shared_ptr<Engine::ModelAsset>);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::string, name);
			);
			
			Static_Bind_Parameters(NewAsset, name);
		);
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(std::shared_ptr<Engine::ModelAsset>);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::string, name);

				Document("");
				Function_Parameter(std::string, path);

				Document("");
				Function_Parameter_Default(LuaEnum<Enum::VertexMode>, mode, Enum::VertexMode::Seperate);
			);
			
			Static_Bind_Parameters(NewAsset, name, path, mode);
		);
	);
);