#include "RigidBody.h"

#include "PointMass.h"

namespace Engine
{
	using Physics::RigidBody;

	Reflect_Inherited(RigidBody, Object,
		Document_Class("");
			
		Document("");
		Archivable Class_Member(Vector3, Velocity);

		Document("");
		Archivable Class_Member(Vector3, Torque);
		
		Bind_Function(AddForce,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(Vector3, force);

					Document("");
					Function_Parameter_Default(Vector3, position, Vector3());
				);

				Bind_Parameters_No_Return(AddForce, force, position);
			);
		);
		
		Bind_Function(GetForceCount,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
	
				Overload_Parameters();

				Bind_Parameters(GetForceCount);
			);
		);
		
		Bind_Function(GetForce,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetForce, index);
			);
		);
		
		Bind_Function(GetPointMassCount,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
	
				Overload_Parameters();

				Bind_Parameters(GetPointMassCount);
			);
		);
		
		Bind_Function(GetPointMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<PointMass>);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetPointMass, index);
			);
		);
		
		Bind_Function(GetMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
	
				Overload_Parameters();

				Bind_Parameters(GetMass);
			);
		);
		
		Bind_Function(GetCenterOfMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters();

				Bind_Parameters(GetCenterOfMass);
			);
		);
		
		Bind_Function(AddMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<PointMass>, mass);
				);

				Bind_Parameters_No_Return(AddMass, mass);
			);
		);
		
		Bind_Function(RemoveMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<PointMass>, mass);
				);

				Bind_Parameters_No_Return(RemoveMass, mass);
			);
		);
		
		Bind_Function(HasMass,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<PointMass>, mass);
				);

				Bind_Parameters(HasMass, mass);
			);
		);
	);
}