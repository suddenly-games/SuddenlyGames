#pragma once

#include <functional>

#include "ShaderPrograms.h"
#include "PointGenerator.h"
#include "Mesh.h"
#include "Aabb.h"
#include "Scene.h"

#undef min
#undef max

namespace GraphicsEngine
{
	class Camera;
	class ModelAsset;

	class Particle
	{
	public:
		float Time;
		float LifeTime;
		Vector3 Position;
		Vector3 Direction;
		Vector3 Size;

	private:

	};

	class EmitterConfig : public Engine::Object
	{
	public:
		RGBA Color = 0xFFFFFFFF;
		RGBA GlowColor = 0xFFFFFFFF;
		Vector3 UVScale = Vector3(1, 1);
		Vector3 UVOffset;
		Vector3 BoxScale = Vector3(1, 1, 1);
		bool CubeMapped = false;
		//bool flip = false;
		bool CompressedNormalMap = false;

		void Initialize() {}
		void Update(float delta) {}

		virtual void operator()();

		Instantiable;

		Inherits_Class(Object);

		Reflected(EmitterConfig);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::EmitterConfig, Engine::Object);
}

namespace GraphicsEngine
{
	class ParticleEmitter : public SceneObject
	{
	public:
		typedef std::function<void()> ConfigureShaderCallback;
		typedef std::function<void(Particle& particle, float delta)> ParticleSpawnCallback;
		typedef std::function<bool(Particle& particle, float delta)> ParticleUpdateCallback;
		typedef std::function<void(const Particle& particle, const std::shared_ptr<Camera>& camera, const Mesh* particleMesh)> ParticleDrawCallback;

		bool Enabled = true;
		int EmitCount = 5;
		float EmitRate = 1;

		std::weak_ptr<Engine::ModelAsset> Asset;

		void SetMaxParticles(int count);
		void Initialize() { Particles.resize(MaxParticles); }
		void Update(float delta);
		void Draw(const std::shared_ptr<Camera>& camera);
		void FireParticles(int count, float delta = 0);
		bool IsTransparent() const;
		Aabb GetBoundingBox() const;
		bool HasMoved() const;
		bool IsStatic() const;

		void SetConfigure(const ConfigureShaderCallback& callback, const std::shared_ptr<Object>& object = nullptr)
		{
			ConfigureCallback = callback;
			ConfigHandle = object->Cast<EmitterConfig>();
		}

		void SetConfigure(const std::shared_ptr<EmitterConfig>& configuration)
		{
			SetConfigure(*configuration, configuration);
		}

		void SetSpawn(const ParticleSpawnCallback& callback, const std::shared_ptr<Object>& object = nullptr)
		{
			SpawnCallback = callback;
			SpawnHandle = object;
		}

		void SetUpdate(const ParticleUpdateCallback& callback, const std::shared_ptr<Object>& object = nullptr)
		{
			UpdateCallback = callback;
			UpdateHandle = object;
		}

		void SetDraw(const ParticleDrawCallback& callback, const std::shared_ptr<Object>& object = nullptr)
		{
			DrawCallback = callback;
			DrawHandle = object;
		}

		static void ConfigureShader();
		static void DefaultSpawn(Particle& particle, float delta);
		static bool DefaultUpdate(Particle& particle, float delta);
		static void DefaultDraw(const Particle& particle, const std::shared_ptr<Camera>& camera, const Mesh* particleMesh);

	private:
		typedef std::vector<Particle> ParticleVector;

		int MaxParticles = 10;
		int ParticlesAlive = 0;
		float TotalLifetime = 0;
		float LastSpawn = 0;
		Aabb BoundingBox;

		ParticleVector Particles;

		std::weak_ptr<EmitterConfig> ConfigHandle;
		std::weak_ptr<Object> SpawnHandle;
		std::weak_ptr<Object> UpdateHandle;
		std::weak_ptr<Object> DrawHandle;
		ConfigureShaderCallback ConfigureCallback = ConfigureShader;
		ParticleSpawnCallback SpawnCallback = DefaultSpawn;
		ParticleUpdateCallback UpdateCallback = DefaultUpdate;
		ParticleDrawCallback DrawCallback = DefaultDraw;

		static Aabb GetPairBounds(const Aabb& first, const Aabb& second);

		Instantiable;

		Inherits_Class(SceneObject);

		Reflected(ParticleEmitter);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::ParticleEmitter, Engine::Object);
}

namespace GraphicsEngine
{
	class ParticleConfiguration : public Engine::Object
	{
	public:
		void Initialize() {}
		void Update(float delta) {}

		virtual void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter) = 0;

		Vector3 RelativeVector(const Vector3& vector) const;
		Vector3 GetMomentum() const;

		NonInstantiable;

		Inherits_Class(Object);

		Reflected(ParticleConfiguration);
	};

	class DirectionalParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Speed;
		NumberRange Lifetime;
		NumberRange Scale;

		Vector3 Size = Vector3(1, 1, 1);
		Vector3 Position;
		UnitVectorGenerator DirectionGenerator;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(DirectionalParticleSpawner);
	};

	class RisingParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Speed;
		NumberRange Lifetime;
		NumberRange Scale;

		Vector3 Size = Vector3(1, 1, 1);
		Vector3 Position;
		Vector3 Direction;

		UnitVectorGenerator DirectionGenerator;

		float Radius = 1;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(RisingParticleSpawner);
	};

	class PlanarParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Distance1;
		NumberRange Distance2;
		NumberRange Scale;
		NumberRange Speed;
		NumberRange Lifetime;

		Vector3 Axis1 = Vector3(1, 0, 0);
		Vector3 Axis2 = Vector3(0, 0, -1);
		Vector3 Size = Vector3(1, 1, 1);
		Vector3 Position;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(PlanarParticleSpawner);
	};

	class LineParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Scale;
		NumberRange Speed;
		NumberRange Lifetime;

		Vector3 Point1;
		Vector3 Point2;
		Vector3 Direction;
		Vector3 Size = Vector3(1, 1, 1);

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(LineParticleSpawner);
	};

	class RingParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Scale;
		NumberRange Speed;
		NumberRange Lifetime;
		NumberRange Radius = NumberRange(1, 1);
		NumberRange Angle;

		Vector3 Position;
		Vector3 Normal = Vector3(0, 1, 0);
		Vector3 Size = Vector3(1, 1, 1);

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(RingParticleSpawner);
	};

	class ConeParticleSpawner : public ParticleConfiguration
	{
	public:
		NumberRange Scale;
		NumberRange Speed;
		NumberRange Lifetime;
		NumberRange Angle;

		Vector3 Position;
		Vector3 Normal;
		Vector3 Size = Vector3(1, 1, 1);

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetSpawn(std::ref(*this), This.lock()->Cast<Object>());
		}

		void operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(ConeParticleSpawner);
	};

	class GravityParticleUpdater : public ParticleConfiguration
	{
	public:
		Vector3 Force;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetUpdate(std::ref(*this), This.lock()->Cast<Object>());
		}

		bool operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(GravityParticleUpdater);
	};

	class DragParticleUpdater : public ParticleConfiguration
	{
	public:
		float DragFactor = 1;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetUpdate(std::ref(*this), This.lock()->Cast<Object>());
		}

		bool operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(DragParticleUpdater);
	};

	class DampenedForceUpdater : public ParticleConfiguration
	{
	public:
		float DragFactor = 1;
		Vector3 Force;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetUpdate(std::ref(*this), This.lock()->Cast<Object>());
		}

		bool operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(DampenedForceUpdater);
	};

	class CubicBezierUpdater : public ParticleConfiguration
	{
	public:
		Vector3 Start;
		Vector3 Control1;
		Vector3 Control2;
		Vector3 End;
		float FinishThreshold = 0.1f;

		void Initialize() {}
		void Update(float delta) {}

		void AttachTo(const std::shared_ptr<ParticleEmitter>& emitter)
		{
			emitter->SetUpdate(std::ref(*this), This.lock()->Cast<Object>());
		}

		bool operator()(Particle& particle, float delta);

		Instantiable;

		Inherits_Class(ParticleConfiguration);

		Reflected(CubicBezierUpdater);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::ParticleConfiguration, Engine::Object);
	//Class_Inherits(GraphicsEngine::DirectionalParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::RisingParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::PlanarParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::LineParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::RingParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::ConeParticleSpawner, Engine::Object);
	//Class_Inherits(GraphicsEngine::GravityParticleUpdater, Engine::Object);
	//Class_Inherits(GraphicsEngine::DragParticleUpdater, Engine::Object);
	//Class_Inherits(GraphicsEngine::DampenedForceUpdater, Engine::Object);
	//Class_Inherits(GraphicsEngine::CubicBezierUpdater, Engine::Object);
}
