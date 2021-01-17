#include "Scene.h"

#include "Terrain.h"
#include "Light.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Scene, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(std::weak_ptr<Camera>, CurrentCamera);

		Document("");
		Archivable Class_Member(std::weak_ptr<Light>, GlobalLight);
	
		Bind_Function(AddObject,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<SceneObject>, object);
				);
			
				Bind_Parameters_No_Return(AddObject, object);
			);
		);
	
		Bind_Function(AddLight,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Light>, object);
				);
			
				Bind_Parameters_No_Return(AddLight, object);
			);
		);
	
		Bind_Function(AddTerrain,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Engine::Terrain>, terrain);
				);
			
				Bind_Parameters_No_Return(AddTerrain, terrain);
			);
		);
	
		Bind_Function(RemoveObject,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<SceneObject>, object);
				);
			
				Bind_Parameters_No_Return(RemoveObject, object);
			);
		);
	
		Bind_Function(RemoveLight,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Light>, object);
				);
			
				Bind_Parameters_No_Return(RemoveLight, object);
			);
		);
	
		Bind_Function(RemoveTerrain,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Engine::Terrain>, terrain);
				);
			
				Bind_Parameters_No_Return(RemoveTerrain, terrain);
			);
		);
	);
}
