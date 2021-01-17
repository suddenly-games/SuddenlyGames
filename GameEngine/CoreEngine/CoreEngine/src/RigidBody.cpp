#include "RigidBody.h"

#include "Transform.h"
#include "PointMass.h"

namespace Engine
{
	namespace Physics
	{
		void RigidBody::Update(float delta)
		{
			std::shared_ptr<Transform> transform = GetComponent<Transform>();

			if (transform == nullptr)
				return;

			float mass = GetMass();
			Vector3 acceleration;

			if (mass != 0)
				for (int i = 0; i < int(ForceBuffer.size()); ++i)
					acceleration += (1 / mass) * ForceBuffer[i];

			transform->Move(transform->Transformation * transform->GetWorldTransformationInverse() * (delta * (Velocity + delta * acceleration)));
			
			Velocity += acceleration;

			ForceBuffer.clear();
		}

		void RigidBody::AddForce(const Vector3& force, const Vector3& position)
		{
			ForceBuffer.push_back(force);
		}

		int RigidBody::GetForceCount() const
		{
			return int(ForceBuffer.size());
		}

		Vector3 RigidBody::GetForce(int index) const
		{
			return ForceBuffer[index];
		}

		int RigidBody::GetPointMassCount() const
		{
			return int(Mass.size());
		}

		std::shared_ptr<PointMass> RigidBody::GetPointMass(int index)
		{
			return Mass[index].lock();
		}

		float RigidBody::GetMass() const
		{
			float total = 0;

			for (int i = 0; i < int(Mass.size()); ++i)
				if (!Mass[i].expired())
					total += Mass[i].lock()->Mass;

			return total;
		}

		Vector3 RigidBody::GetCenterOfMass() const
		{
			if (Mass.size() == 0)
				return Vector3();

			float mass = 0;
			Vector3 center;

			for (int i = 0; i < int(Mass.size()); ++i)
			{
				if (!Mass[i].expired())
				{
					auto currentMass = Mass[i].lock();

					mass += currentMass->Mass;
					center += currentMass->Mass * currentMass->Position;
				}
			}

			if (mass == 0)
				return center;

			return (1 / mass) * center;
		}

		void RigidBody::ProcessDisplacement(const Vector3& displacement, const Vector3& position)
		{
			std::shared_ptr<Transform> transform = GetComponent<Transform>();

			if (transform == nullptr)
				return;

			transform->Move(transform->Transformation * transform->GetWorldTransformationInverse() * displacement);
		}

		void RigidBody::AddMass(const std::shared_ptr<PointMass>& mass)
		{
			Mass.push_back(mass);
			Mass.back() = mass;
		}

		void RigidBody::RemoveMass(const std::shared_ptr<PointMass>& mass)
		{
			for (int i = 0; i < int(Mass.size()); ++i)
			{
				if (Mass[i].lock() == mass)
				{
					std::swap(Mass[i], Mass.back());
					Mass.pop_back();
				}
			}
		}

		bool RigidBody::HasMass(const std::shared_ptr<PointMass>& mass) const
		{
			for (int i = 0; i < int(Mass.size()); ++i)
				if (Mass[i].lock() == mass)
					return true;

			return false;
		}
	}
}