#include "ParticleEmitter.h"

#include "PointGenerator.h"
#include "Graphics.h"
#include "MeshLoader.h"
#include "Transform.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelAsset.h"
#include "RigidBody.h"

namespace GraphicsEngine
{
	void ParticleEmitter::SetMaxParticles(int count)
	{
		MaxParticles = count;
		Particles.resize(MaxParticles);
		std::cout << "Max Particles: " << MaxParticles << " | " << Particles.size() << std::endl;
	}

	Aabb ParticleEmitter::GetPairBounds(const Aabb& first, const Aabb& second)
	{
		Vector3 minCorner = first.Min;
		Vector3 maxCorner = first.Max;

		if (second.Min.X < minCorner.X)
			minCorner.X = second.Min.X;

		if (second.Min.Y < minCorner.Y)
			minCorner.Y = second.Min.Y;

		if (second.Min.Z < minCorner.Z)
			minCorner.Z = second.Min.Z;

		if (second.Max.X > maxCorner.X)
			maxCorner.X = second.Max.X;

		if (second.Max.Y > maxCorner.Y)
			maxCorner.Y = second.Max.Y;

		if (second.Max.Z > maxCorner.Z)
			maxCorner.Z = second.Max.Z;

		return Aabb(minCorner, maxCorner);
	}

	void ParticleEmitter::Update(float delta)
	{
		if (Enabled)
			TotalLifetime += delta;

		if (TotalLifetime - LastSpawn >= EmitRate)
		{
			int bursts = (int)((TotalLifetime - LastSpawn) / EmitRate);

			LastSpawn += bursts * EmitRate;

			for (int i = 0; i < bursts; ++i)
				FireParticles(EmitCount, delta - i * EmitRate);
		}

		if (ParticlesAlive > 0)
			BoundingBox = Aabb(Particles[0].Position - 0.5f * Particles[0].Size, Particles[0].Position + 0.5f * Particles[0].Size);
		else
			BoundingBox = Aabb();

		for (int i = 0; i < ParticlesAlive; ++i)
		{
			bool alive = UpdateCallback(Particles[i], delta);

			if (!alive)
			{
				--ParticlesAlive;

				Particles[i] = Particles[ParticlesAlive];

				--i;
			}
			else
				BoundingBox = GetPairBounds(BoundingBox, Aabb(Particles[i].Position - 0.5f * Particles[i].Size, Particles[i].Position + 0.5f * Particles[i].Size));
		}
	}

	void ParticleEmitter::Draw(const std::shared_ptr<Camera>& camera)
	{
		const Mesh* mesh = ShaderProgram::GetCurrentProgram()->Meshes->GetMesh(Asset.lock()->GetMeshID());

		if (mesh == nullptr)
			return;

		// inlined for optimization
		glBindVertexArray(mesh->GetMeshID());

		ConfigureCallback();

		for (int i = 0; i < ParticlesAlive; ++i)
			DrawCallback(Particles[i], camera, mesh);
	}

	void ParticleEmitter::FireParticles(int count, float delta)
	{
		for (int j = 0; j < count && ParticlesAlive < MaxParticles; ++j)
		{
			SpawnCallback(Particles[ParticlesAlive], delta);

			++ParticlesAlive;
		}
	}

	bool ParticleEmitter::IsTransparent() const
	{
		return ConfigHandle.expired() || ConfigHandle.lock()->Color.A != 1;
	}

	Aabb ParticleEmitter::GetBoundingBox() const
	{
		return BoundingBox;
	}

	bool ParticleEmitter::HasMoved() const
	{
		return true;
	}

	bool ParticleEmitter::IsStatic() const
	{
		return false;
	}

	void ParticleEmitter::ConfigureShader()
	{

	}

	void ParticleEmitter::DefaultSpawn(Particle& particle, float delta)
	{
		particle.Time = 10 - delta;
		particle.Position.Set(0, 0, -1, 1);
		particle.Direction = 0.1f * UnitVectorGenerator().Generate();//.Set(0, 0, -0.1f);
		particle.Size.Set(.1f, .1f, .1f);
	}

	bool ParticleEmitter::DefaultUpdate(Particle& particle, float delta)
	{
		particle.Time -= delta;
		particle.Position += delta * particle.Direction;

		return particle.Time > 0;
	}

	void ParticleEmitter::DefaultDraw(const Particle& particle, const std::shared_ptr<Camera>& camera, const Mesh* particleMesh)
	{
		//Vector3 translation = camera->GetTransformationInverse() * particle.Position;
		//
		//program->SetTransform(camera->GetProjectionMatrix() * Matrix3().Translate(translation) * Matrix3().Scale(particle.size()));

		// v this is equivalent to this ^ but faster
		Matrix3 inv = camera->GetTransformationInverse();

		Vector3 translation = Vector3(
			inv[0][0] * particle.Position.X + inv[0][1] * particle.Position.Y + inv[0][2] * particle.Position.Z + inv[0][3],
			inv[1][0] * particle.Position.X + inv[1][1] * particle.Position.Y + inv[1][2] * particle.Position.Z + inv[1][3],
			inv[2][0] * particle.Position.X + inv[2][1] * particle.Position.Y + inv[2][2] * particle.Position.Z + inv[2][3],
			1
		);

		Matrix3 persp = camera->GetProjectionMatrix();
		persp[0][3] = persp[0][0] * translation.X;
		persp[1][3] = persp[1][1] * translation.Y;
		persp[2][3] = persp[2][2] * translation.Z + persp[2][3];
		persp[3][3] = -translation.Z;
		persp[0][0] *= particle.Size.X;
		persp[1][1] *= particle.Size.Y;
		persp[2][2] *= particle.Size.Z;
		persp[3][2] *= particle.Size.Z;

		ShaderProgram* current = ShaderProgram::GetCurrentProgram();

		if (current == Programs::Phong)
		{
			Programs::Phong->useOffsetMap.Set(false);
			Programs::Phong->transform.Set(persp);
			Programs::Phong->objectTransform.Set(translation);
		}
		else if (current == Programs::PhongForward)
		{
			Programs::PhongForward->useOffsetMap.Set(false);
			Programs::PhongForward->transform.Set(persp);
			Programs::PhongForward->objectTransform.Set(translation);
		}
		else if (current == Programs::ShadowMap)
		{
			Programs::ShadowMap->transform.Set(persp);
			Programs::ShadowMap->objectZTransform.Set(0, 0, 0, translation.Z);
		}
		else if (current == Programs::DepthTrace)
		{
			Programs::DepthTrace->transform.Set(persp);
			Programs::DepthTrace->objectTransform.Set(translation);
		}
		//Programs::Phong->objectTransform.Set(Matrix3().SetVectors(particle.Position, camera->GetTransformation().RightVector(), camera->GetTransformation().UpVector(), camera->GetTransformation().FrontVector()));

		//particleMesh->Draw();

		// inlined for optimization
		glDrawElements(GL_TRIANGLES, particleMesh->GetIndicesID(), GL_UNSIGNED_INT, 0);
	}

	void EmitterConfig::operator()()
	{
		ShaderProgram* current = ShaderProgram::GetCurrentProgram();

		if (current == Programs::Phong)
		{
			Programs::Phong->SetTexture(nullptr);
			Programs::Phong->SetNormalMap(nullptr);
			Programs::Phong->SetSpecularMap(nullptr);
			//Programs::Phong->globalEmissive.Set(0);;
			Programs::Phong->textureColor.Set(0xFFFFFFFF);
			Programs::Phong->blendTexture.Set(false);
			Programs::Phong->compressedNormalMap.Set(CompressedNormalMap);
			Programs::Phong->singleCubeMapped.Set(CubeMapped);
			Programs::Phong->flipCubeMapV.Set(false);
			Programs::Phong->boxScale.Set(BoxScale);
			Programs::Phong->uvScale.Set(UVScale);
			Programs::Phong->uvOffset.Set(UVOffset);
		}
		else if (current == Programs::PhongForward)
		{
			Programs::PhongForward->SetTexture(nullptr);
			//Programs::Phong->globalEmissive.Set(0);
			Programs::PhongForward->color.Set(Color);
			Programs::PhongForward->glowColor.Set(GlowColor);
			Programs::PhongForward->SetTexture(nullptr);
			Programs::PhongForward->SetNormalMap(nullptr);
			Programs::PhongForward->SetSpecularMap(nullptr);
			Programs::PhongForward->textureColor.Set(0xFFFFFFFF);
			Programs::PhongForward->blendTexture.Set(false);
			//Programs::PhongForward->flipCubeMapV.Set(flipCubeMapV);
			Programs::PhongForward->singleCubeMapped.Set(CubeMapped);
			Programs::PhongForward->compressedNormalMap.Set(CompressedNormalMap);
			Programs::PhongForward->boxScale.Set(BoxScale);
			Programs::PhongForward->uvScale.Set(UVScale);
			Programs::PhongForward->uvOffset.Set(UVOffset);
		}
	}

	Vector3 ParticleConfiguration::RelativeVector(const Vector3& vector) const
	{
		std::shared_ptr<Engine::Transform> transform = GetComponent<Engine::Transform>();

		if (transform == nullptr)
			return vector;

		return transform->GetWorldTransformation() * vector;
	}

	Vector3 ParticleConfiguration::GetMomentum() const
	{
		std::shared_ptr<Engine::Physics::RigidBody> rigidBody = GetComponent<Engine::Physics::RigidBody>();

		if (rigidBody == nullptr)
			return Vector3();

		return rigidBody->Velocity;
	}

	void DirectionalParticleSpawner::operator()(Particle& particle, float delta)
	{
		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Direction = RelativeVector(Speed.rand() * DirectionGenerator.Generate()) + GetMomentum();
		particle.Position = RelativeVector(Position + delta * particle.Direction);
		particle.Size = 0.05f * Scale.rand() * Size;
	}

	void RisingParticleSpawner::operator()(Particle& particle, float delta)
	{
		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Position = RelativeVector(Position + Radius * cbrt(VectorGenerator::randf()) * DirectionGenerator.Generate());
		particle.Direction = RelativeVector(Speed.rand() * Direction) + GetMomentum();
		particle.Size = 0.05f * Scale.rand() * Size;
	}

	void PlanarParticleSpawner::operator()(Particle& particle, float delta)
	{
		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Direction = RelativeVector(Speed.rand() * Axis1.Cross(Axis2).Normalize()) + GetMomentum();
		particle.Position = RelativeVector(Position + Distance1.rand() * Axis1 + Distance2.rand() * Axis2);
		particle.Size = Scale.rand() * Size;
	}

	void LineParticleSpawner::operator()(Particle& particle, float delta)
	{
		float t = NumberRange(0, 1).rand();

		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Direction = RelativeVector(Speed.rand() * Direction) + GetMomentum();
		particle.Position = RelativeVector((1 - t) * Point1 + t * Point2);
		particle.Size = Scale.rand() * Size;
	}

	void RingParticleSpawner::operator()(Particle& particle, float delta)
	{
		float scalar = (1 - Normal.Y) / (Normal.X * Normal.X + Normal.Z * Normal.Z);

		if (abs(Normal.X * Normal.Z) < 1e-5f)
			scalar = 1;

		Vector3 vector1(
			scalar * Normal.Z * Normal.Z + Normal.Y,
			-Normal.X,
			-scalar * Normal.X * Normal.Z
		);

		Vector3 vector2(-vector1.Z, Normal.Z, Normal.X * vector1.Y - Normal.Y * vector1.X);

		float theta = NumberRange(0, 2 * PI).rand();
		float distance = Radius.rand();

		Vector3 direction = cosf(theta) * vector1 + sinf(theta) * vector2;

		float theta2 = Angle.rand();

		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Direction = RelativeVector(Speed.rand() * (cosf(theta2) * direction + sinf(theta2) * Normal)) + GetMomentum();
		particle.Position = RelativeVector(distance * direction + Position);
		particle.Size = Scale.rand() * Size;
	}

	void ConeParticleSpawner::operator()(Particle& particle, float delta)
	{
		float scalar = (1 - Normal.Y) / (Normal.X * Normal.X + Normal.Z * Normal.Z);

		if (abs(Normal.X * Normal.Z) < 1e-5f)
			scalar = 1;

		Vector3 vector1(
			scalar * Normal.Z * Normal.Z + Normal.Y,
			-Normal.X,
			-scalar * Normal.X * Normal.Z
		);

		Vector3 vector2(-vector1.Z, Normal.Z, Normal.X * vector1.Y - Normal.Y * vector1.X);

		float theta = NumberRange(0, 2 * PI).rand();

		Vector3 direction = cosf(theta) * vector1 + sinf(theta) * vector2;

		float rangeStart = Angle.Min / Angle.Max;

		float theta2 = Angle.Min + (Angle.Max - Angle.Min) * acosf(rangeStart + VectorGenerator::randf() * (1 - rangeStart)) * 2 / PI;

		particle.LifeTime = Lifetime.rand();
		particle.Time = particle.LifeTime - delta;
		particle.Direction = RelativeVector(Speed.rand() * (cosf(theta2) * Normal + sinf(theta2) * direction)) + GetMomentum();
		particle.Position = RelativeVector(Position);
		particle.Size = Scale.rand() * Size;
	}

	bool GravityParticleUpdater::operator()(Particle& particle, float delta)
	{
		particle.Time -= delta;
		particle.Direction += delta * Force;
		particle.Position += delta * particle.Direction;

		return particle.Time > 0;
	}

	bool DragParticleUpdater::operator()(Particle& particle, float delta)
	{
		particle.Time -= delta;
		particle.Direction *= DragFactor;
		particle.Position += delta * particle.Direction;

		return particle.Time > 0;
	}

	bool DampenedForceUpdater::operator()(Particle& particle, float delta)
	{
		particle.Time -= delta;
		particle.Direction *= DragFactor;
		particle.Direction += delta * Force;
		particle.Position += delta * particle.Direction;

		return particle.Time > 0;
	}

	bool CubicBezierUpdater::operator()(Particle& particle, float delta)
	{
		particle.Time -= delta;

		float t = (particle.LifeTime - particle.Time) / particle.LifeTime;

		if (t > 1)
			t = 1;

		float t2 = 1 - t;

		Vector3 point = t2 * t2*t2 * Start + 3 * t2*t2*t * Control1 + 3 * t2*t*t * Control2 + t * t*t * End;

		particle.Direction = /* 0.5f * particle.Direction + 0.5f */ (point - particle.Position);

		float s = 1 / (particle.Position - End).SquareLength();

		if (s > 0.1f)
			s = 0.1f;

		if (s < 1e-3f)
			s = 1e-3f;

		particle.Position = (1 - s) * particle.Position + s * point;

		return particle.Time > 0;//(particle.Position - end).SquareLength() > finishThreshold;//particle.Time > 0;
	}
}