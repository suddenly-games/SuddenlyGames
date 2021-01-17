#pragma once

#include "Matrix3.h"
#include "Object.h"
#include "DeviceVector.h"

namespace GraphicsEngine
{
	class DeviceTransform : public Engine::Object
	{
	public:
		struct AlignmentEnum
		{
			enum Alignment
			{
				Minimum,
				Center,
				Maximum
			};
		};

		typedef AlignmentEnum::Alignment Alignment;

		bool Enabled = true;
		bool Visible = true;
		float AspectRatio = 0;
		Alignment AlignX = Alignment::Minimum;
		Alignment AlignY = Alignment::Minimum;
		DeviceVector Size = DeviceVector(1, 0, 1, 0);
		DeviceVector Position = DeviceVector(0, 0, 0, 0);

		~DeviceTransform();

		void Initialize();
		void Update(float);

		void Draw(bool updateStencils  = false);
		void Clip();

		Matrix3 GetTransformation() const;
		Matrix3 GetTransformation();
		Matrix3 GetInverseTransformation() const;
		Matrix3 GetInverseTransformation();

		Vector3 GetAbsoluteSize() const;
		Vector3 GetAbsoluteSize();

		Vector3 GetAbsolutePosition(const DeviceVector& point = DeviceVector(0, 0, 0, 0)) const;
		Vector3 GetAbsolutePosition(const DeviceVector& point = DeviceVector(0, 0, 0, 0));

		Vector3 GetLocalPosition(const DeviceVector& point) const;
		Vector3 GetLocalPosition(const DeviceVector& point);
		Vector3 GetLocalPosition(const Vector3& point) const;
		Vector3 GetLocalPosition(const Vector3& point);

		Vector3 GetResolution() const;

		bool ContainsLocalPoint(const Vector3& point) const;
		bool ContainsLocalPoint(const Vector3& point);

		bool ContainsPoint(const Vector3& point) const;
		bool ContainsPoint(const Vector3& point);

		bool HasMoved() const;
		bool IsVisible() const;
		bool IsEnabled() const;

		void UpdateTransformation();

	private:
		DeviceVector LastSize = DeviceVector(1, 0, 1, 0);
		DeviceVector LastPosition = DeviceVector(0, 0, 0, 0);
		Vector3 AbsoluteSize;
		Matrix3 Transformation;
		Matrix3 InverseTransformation;

		void Draw(std::shared_ptr<Object> object, bool updateStencils);

		Instantiable;

		Inherits_Class(Object);

		Reflected(DeviceTransform);
	};
}

namespace Enum
{
	typedef GraphicsEngine::DeviceTransform::Alignment Alignment;
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::DeviceTransform, Object);

	Declare_Enum(Alignment);
}