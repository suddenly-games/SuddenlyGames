#include "Transform.h"

namespace Engine
{
	void Transform::Update(float delta)
	{
		Recompute();

		Moved = false;
	}

	void Transform::Recompute()
	{
		if (!HasChanged())
			return;

		Moved = true;

		std::shared_ptr<Transform> inherited;

		if (InheritTransformation)
			inherited = GetComponent<Transform>(true);

		HadParent = InheritTransformation && inherited != nullptr;

		if (inherited == nullptr)
			WorldTransformation = Transformation;
		else
			WorldTransformation = inherited->GetWorldTransformation() * Transformation;

		WorldTransformationInverse.Invert(WorldTransformation);
		WorldNormalTransformation = WorldTransformation.Inverted().Transpose();

		OldTransform = Transformation;

		if (HadParent)
			OldParentTransform = inherited->GetWorldTransformation();
	}

	bool Transform::HasMoved() const
	{
		if (Moved)
			return true;
		else
		{
			std::shared_ptr<Transform> parent = GetComponent<Transform>();

			if (parent != nullptr && parent->HasMoved())
				return true;
		}

		return Transformation != OldTransform;
	}

	bool Transform::HasMoved()
	{
		if (Moved)
			return true;
		else
		{
			std::shared_ptr<Transform> parent = GetComponent<Transform>();

			if (parent != nullptr && parent->HasMoved())
			{
				Moved = true;

				return true;
			}
		}

		Moved = Transformation != OldTransform;

		return Moved;
	}

	bool Transform::HasChanged()
	{
		std::shared_ptr<Transform> parent = GetComponent<Transform>();

		if (InheritTransformation)
		{
			bool hasParent = parent != nullptr;

			if (hasParent != HadParent)
				return true;
			else if (hasParent && parent->GetWorldTransformation() != OldParentTransform)
				return true;
		}

		return Transformation != OldTransform;
			//return true;

		
		//return false;
		//Handle<Transform> parent = GetComponent<Transform>();
		//
		//if (parent != nullptr && parent->HasChanged())
		//	return true;
		//
		//return Transformation != OldTransform;
	}

	Vector3 Transform::GetPosition() const
	{
		return Transformation.Translation();
	}

	Vector3 Transform::GetPosition()
	{
		Recompute();

		return Transformation.Translation();
	}

	void Transform::SetPosition(const Vector3& position)
	{
		Transformation.SetTranslation(position);
	}

	void Transform::Move(const Vector3& offset)
	{
		Transformation.SetTranslation(Transformation.Translation() + offset);
	}

	Vector3 Transform::GetWorldPosition() const
	{
		return WorldTransformation.Translation();
	}

	Vector3 Transform::GetWorldPosition()
	{
		//Handle<Transform> parent = GetComponent<Transform>();
		//if (parent != nullptr)
		//	return (parent->GetWorldTransformation() * Transformation).Translation();
		//
		//return Transformation.Translation();

		Recompute();
		
		return WorldTransformation.Translation();
	}

	const Matrix3& Transform::GetWorldTransformation() const
	{
		return WorldTransformation;
	}

	const Matrix3& Transform::GetWorldTransformation()
	{
		//Recompute();
		//Handle<Transform> parent = GetComponent<Transform>();
		//if (parent != nullptr)
		//	return parent->GetWorldTransformation() * Transformation;
		//
		//return Transformation;
		Recompute();
		
		return WorldTransformation;
	}

	const Matrix3& Transform::GetWorldTransformationInverse() const
	{
		return WorldTransformationInverse;
	}

	const Matrix3& Transform::GetWorldTransformationInverse()
	{
		Recompute();

		return WorldTransformationInverse;
	}

	Quaternion Transform::GetWorldOrientation() const
	{
		return Quaternion(WorldTransformation);
	}

	Quaternion Transform::GetWorldOrientation()
	{
		Recompute();

		return Quaternion(WorldTransformation);
	}

	const Matrix3& Transform::GetWorldNormalTransformation() const
	{
		return WorldNormalTransformation;
	}

	const Matrix3& Transform::GetWorldNormalTransformation()
	{
		Recompute();

		return WorldNormalTransformation;
	}

	Quaternion Transform::GetOrientation() const
	{
		return Quaternion(Transformation);
	}

	Quaternion Transform::GetOrientation()
	{
		Recompute();

		return Quaternion(Transformation);
	}

	void Transform::SetOrientation(const Quaternion& orientation)
	{
		Transformation = Matrix3(orientation).SetTranslation(Transformation.Translation());
	}

	void Transform::Rotate(const Quaternion& rotation)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());
		
		Transformation = (Matrix3(rotation) * Transformation).SetTranslation(translation);
	}

	void Transform::Rotate(const Vector3 axis, float angle)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3().RotateAxis(axis, angle) * Transformation).SetTranslation(translation);
	}

	Vector3 Transform::GetEulerAngles() const
	{
		return Vector3();
	}

	Vector3 Transform::GetEulerAngles()
	{
		Recompute();

		return Vector3();
	}

	void Transform::SetEulerAngles(const Vector3& angles)
	{
		Transformation = Matrix3::EulerAnglesRotation(angles.X, angles.Y, angles.Z).SetTranslation(Transformation.Translation());
	}

	void Transform::SetEulerAngles(float pitch, float roll, float yaw)
	{
		Transformation = Matrix3::EulerAnglesRotation(pitch, roll, yaw).SetTranslation(Transformation.Translation());
	}

	void Transform::Rotate(const Vector3& angles)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3::EulerAnglesRotation(angles.X, angles.Y, angles.Z) * Transformation).SetTranslation(translation);
	}

	void Transform::Rotate(float pitch, float roll, float yaw)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3::EulerAnglesRotation(pitch, roll, yaw) * Transformation).SetTranslation(translation);
	}

	Vector3 Transform::GetEulerAnglesYaw() const
	{
		return Vector3();
	}

	Vector3 Transform::GetEulerAnglesYaw()
	{
		Recompute();

		return Vector3();
	}

	void Transform::SetEulerAnglesYaw(float yaw, float pitch, float roll)
	{
		Transformation = Matrix3::EulerAnglesYawRotation(yaw, pitch, roll).SetTranslation(Transformation.Translation());
	}

	void Transform::SetEulerAnglesYaw(const Vector3& angles)
	{
		Transformation = Matrix3::EulerAnglesYawRotation(angles.X, angles.Y, angles.Z).SetTranslation(Transformation.Translation());
	}

	void Transform::RotateYaw(const Vector3& angles)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3::EulerAnglesYawRotation(angles.X, angles.Y, angles.Z) * Transformation).SetTranslation(translation);
	}

	void Transform::RotateYaw(float yaw, float pitch, float roll)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3::EulerAnglesYawRotation(yaw, pitch, roll) * Transformation).SetTranslation(translation);
	}

	Vector3 Transform::GetScale() const
	{
		return Vector3(Transformation.RightVector().Length(), Transformation.UpVector().Length(), Transformation.FrontVector().Length());
	}

	Vector3 Transform::GetScale()
	{
		Recompute();

		return Vector3(Transformation.RightVector().Length(), Transformation.UpVector().Length(), Transformation.FrontVector().Length());
	}

	void Transform::SetScale(const Vector3& scale)
	{
		Transformation.SetRight(Transformation.RightVector().Normalize() * scale.X);
		Transformation.SetUp(Transformation.UpVector().Normalize() * scale.Y);
		Transformation.SetFront(Transformation.FrontVector().Normalize() * scale.Z);
	}

	void Transform::Rescale(const Vector3& scale)
	{
		Transformation.SetRight(Transformation.RightVector() * scale.X);
		Transformation.SetUp(Transformation.UpVector() * scale.Y);
		Transformation.SetFront(Transformation.FrontVector() * scale.Z);
	}

	void Transform::TransformBy(const Matrix3& transformation)
	{
		Transformation = transformation * Transformation;
	}

	void Transform::TransformBy(const Quaternion& transformation, const Vector3& point)
	{
		Transformation = Matrix3(transformation).SetTranslation(point) * Transformation;
	}

	void Transform::TransformByRelative(const Matrix3& transformation)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (transformation * Transformation).SetTranslation(translation);
	}

	void Transform::TransformByRelative(const Quaternion& transformation, const Vector3& point)
	{
		Vector3 translation = Transformation.Translation();

		Transformation.SetTranslation(Vector3());

		Transformation = (Matrix3(transformation).SetTranslation(point) * Transformation).SetTranslation(translation);
	}
}
