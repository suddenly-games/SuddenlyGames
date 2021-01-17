#pragma once

#include "Object.h"
#include "Matrix3.h"

namespace Engine
{
	namespace Physics
	{
		class PointMass;

		class RigidBody : public Object
		{
		public:
			Vector3 Velocity;
			Vector3 Torque;
			
			virtual ~RigidBody() {}

			void Initialize() {}
			void Update(float delta);

			void AddForce(const Vector3& force, const Vector3& position = Vector3());
			int GetForceCount() const;
			Vector3 GetForce(int index) const;
			int GetPointMassCount() const;
			std::shared_ptr<PointMass> GetPointMass(int index);
			float GetMass() const;
			Vector3 GetCenterOfMass() const;
			void ProcessDisplacement(const Vector3& displacement, const Vector3& position = Vector3());
			void AddMass(const std::shared_ptr<PointMass>& mass);
			void RemoveMass(const std::shared_ptr<PointMass>& mass);
			bool HasMass(const std::shared_ptr<PointMass>& mass) const;

		private:
			typedef std::vector<Vector3> CoordinateVector;
			typedef std::vector<std::weak_ptr<PointMass>> PointMassVector;

			CoordinateVector ForceBuffer;
			PointMassVector Mass;

			Instantiable;

			Inherits_Class(Object);

			Reflected(RigidBody);
		};
	}
}