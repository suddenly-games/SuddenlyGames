#include "ParticleEmitter.h"

#include "Camera.h"
#include "Scene.h"
#include "ModelAsset.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(EmitterConfig, Object,
		Document_Class("");
		
		Document("");
		Class_Member(RGBA, Color);

		Document("");
		Class_Member(RGBA, GlowColor);

		Document("");
		Class_Member(Vector3, UVScale);

		Document("");
		Class_Member(Vector3, UVOffset);

		Document("");
		Class_Member(Vector3, BoxScale);

		Document("");
		Class_Member(bool, CubeMapped);

		Document("");
		Class_Member(bool, CompressedNormalMap);
	);

	Reflect_Inherited(ParticleEmitter, SceneObject,
		Document_Class("");
		
		Document("");
		Class_Member(bool, Enabled);

		Document("");
		Class_Member(int, EmitCount);

		Document("");
		Class_Member(float, EmitRate);

		Document("");
		Class_Member(std::weak_ptr<Engine::ModelAsset>, Asset);

		Bind_Function(SetConfigure,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<EmitterConfig>, configuration);
				);

				Bind_Parameters_No_Return(SetConfigure, configuration);
			)
		);

		Bind_Function(SetMaxParticles,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, count);
				);

				Bind_Parameters_No_Return(SetMaxParticles, count);
			)
		);

		Bind_Function(FireParticles,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, count);

					Document("");
					Function_Parameter_Default(float, delta, 0);
				);

				Bind_Parameters_No_Return(FireParticles, count, delta);
			)
		);
	);

	Reflect_Inherited(ParticleConfiguration, Object,
		Document_Class("");

		Bind_Function(AttachTo,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<ParticleEmitter>, emitter);
				);

				Bind_Parameters_No_Return(AttachTo, emitter);
			)
		);
	);

	Reflect_Inherited(DirectionalParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(Vector3, Size);

		Document("");
		Class_Member(Vector3, Position);

		Document("");
		Class_Member(UnitVectorGenerator, DirectionGenerator);
	);

	Reflect_Inherited(RisingParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(Vector3, Size);

		Document("");
		Class_Member(Vector3, Position);

		Document("");
		Class_Member(Vector3, Direction);

		Document("");
		Class_Member(UnitVectorGenerator, DirectionGenerator);

		Document("");
		Class_Member(float, Radius);
	);

	Reflect_Inherited(PlanarParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Distance1);
		
		Document("");
		Class_Member(NumberRange, Distance2);
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(Vector3, Axis1);

		Document("");
		Class_Member(Vector3, Axis2);

		Document("");
		Class_Member(Vector3, Size);

		Document("");
		Class_Member(Vector3, Position);
	);

	Reflect_Inherited(LineParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(Vector3, Point1);

		Document("");
		Class_Member(Vector3, Point2);

		Document("");
		Class_Member(Vector3, Size);

		Document("");
		Class_Member(Vector3, Direction);
	);

	Reflect_Inherited(RingParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(NumberRange, Radius);

		Document("");
		Class_Member(NumberRange, Angle);

		Document("");
		Class_Member(Vector3, Position);

		Document("");
		Class_Member(Vector3, Normal);

		Document("");
		Class_Member(Vector3, Size);
	);

	Reflect_Inherited(ConeParticleSpawner, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(NumberRange, Speed);

		Document("");
		Class_Member(NumberRange, Lifetime);

		Document("");
		Class_Member(NumberRange, Scale);

		Document("");
		Class_Member(NumberRange, Angle);

		Document("");
		Class_Member(Vector3, Position);

		Document("");
		Class_Member(Vector3, Normal);

		Document("");
		Class_Member(Vector3, Size);
	);

	Reflect_Inherited(GravityParticleUpdater, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(Vector3, Force);
	);

	Reflect_Inherited(DragParticleUpdater, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(float, DragFactor);
	);

	Reflect_Inherited(DampenedForceUpdater, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(float, DragFactor);

		Document("");
		Class_Member(Vector3, Force);
	);

	Reflect_Inherited(CubicBezierUpdater, ParticleConfiguration,
		Document_Class("");
		
		Document("");
		Class_Member(Vector3, Start);

		Document("");
		Class_Member(Vector3, Control1);

		Document("");
		Class_Member(Vector3, Control2);

		Document("");
		Class_Member(Vector3, End);

		Document("");
		Class_Member(float, FinishThreshold);
	);
}
