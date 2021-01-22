#include "DeviceTransform.h"

#include "ScreenCanvas.h"
#include "CanvasStencil.h"
#include "TextCanvas.h"

namespace GraphicsEngine
{
	DeviceTransform::~DeviceTransform()
	{

	}

	void DeviceTransform::Initialize()
	{
	}

	void DeviceTransform::Update(float)
	{
		UpdateTransformation();
	}

	void DeviceTransform::Draw(bool updateStencils)
	{
		Draw(This.lock()->Cast<Object>(), updateStencils);
	}

	Matrix3 DeviceTransform::GetTransformation() const
	{
		return Transformation;
	}

	Matrix3 DeviceTransform::GetTransformation()
	{
		UpdateTransformation();

		return Transformation;//((const DeviceTransform*)this)->GetTransformation();
	}

	Matrix3 DeviceTransform::GetInverseTransformation() const
	{
		return InverseTransformation;
	}

	Matrix3 DeviceTransform::GetInverseTransformation()
	{
		UpdateTransformation();

		return InverseTransformation;// ((const DeviceTransform*)this)->GetInverseTransformation();
	}

	Vector3 DeviceTransform::GetAbsoluteSize() const
	{
		return AbsoluteSize;
	}

	Vector3 DeviceTransform::GetAbsoluteSize()
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->GetAbsoluteSize();
	}

	Vector3 DeviceTransform::GetAbsolutePosition(const DeviceVector& point) const
	{
		return Transformation * GetLocalPosition(point);
	}

	Vector3 DeviceTransform::GetAbsolutePosition(const DeviceVector& point)
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->GetAbsolutePosition(point);
	}

	Vector3 DeviceTransform::GetLocalPosition(const DeviceVector& point) const
	{
		return Vector3(
			point.X.Scale * AbsoluteSize.X + point.X.Offset,
			point.Y.Scale * AbsoluteSize.Y + point.Y.Offset,
			0,
			1
		);
	}

	Vector3 DeviceTransform::GetLocalPosition(const DeviceVector& point)
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->GetLocalPosition(point);
	}

	Vector3 DeviceTransform::GetLocalPosition(const Vector3& point) const
	{
		return InverseTransformation * point;
	}

	Vector3 DeviceTransform::GetLocalPosition(const Vector3& point)
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->GetLocalPosition(point);
	}

	Vector3 DeviceTransform::GetResolution() const
	{
		std::shared_ptr<DeviceTransform> parent = GetComponent<DeviceTransform>();

		if (parent != nullptr)
			return parent->GetResolution();

		return AbsoluteSize;
	}

	bool DeviceTransform::ContainsLocalPoint(const Vector3& point) const
	{
		return (point.X >= 0 && point.X < AbsoluteSize.X) && (point.Y >= 0 && point.Y < AbsoluteSize.Y);
	}

	bool DeviceTransform::ContainsLocalPoint(const Vector3& point)
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->ContainsLocalPoint(point);
	}

	bool DeviceTransform::ContainsPoint(const Vector3& point) const
	{
		return ContainsLocalPoint(GetLocalPosition(point));
	}

	bool DeviceTransform::ContainsPoint(const Vector3& point)
	{
		UpdateTransformation();

		return ((const DeviceTransform*)this)->ContainsPoint(point);
	}

	bool DeviceTransform::HasMoved() const
	{
		if (Size != LastSize || Position != LastPosition || Rotation != LastRotation || AnchorPoint != LastAnchorPoint || RotationAnchor != LastRotationAnchor)
			return true;

		std::shared_ptr<DeviceTransform> parent = GetComponent<DeviceTransform>();

		if (parent != nullptr && parent->HasMoved())
			return true;

		return false;
	}

	bool DeviceTransform::IsVisible() const
	{
		if (!Visible)
			return false;

		std::shared_ptr<DeviceTransform> parent = GetComponent<DeviceTransform>();

		if (parent != nullptr)
			return parent->IsVisible();

		return Visible;
	}

	bool DeviceTransform::IsEnabled() const
	{
		if (!Enabled)
			return false;

		std::shared_ptr<DeviceTransform> parent = GetComponent<DeviceTransform>();

		if (parent != nullptr)
			return parent->IsEnabled();

		return Enabled;
	}

	void DeviceTransform::UpdateTransformation()
	{
		if (!HasMoved())
			return;

		Vector3 parentSize;
		Matrix3 parentTransformation;

		std::shared_ptr<DeviceTransform> parent = GetComponent<DeviceTransform>();

		if (parent != nullptr)
		{
			parentSize = parent->GetAbsoluteSize();
			parentTransformation = parent->GetTransformation();
		}

		AbsoluteSize = Vector3(
			parentSize.X * Size.X.Scale + Size.X.Offset,
			parentSize.Y * Size.Y.Scale + Size.Y.Offset
		);

		Vector3 scale = AbsoluteSize;
		Vector3 translation = Vector3(
			2 * Position.X.Offset,
			2 * Position.Y.Offset
		) - (parentSize - AbsoluteSize);

		translation = translation.Scale(1, -1, 1);

		if (parent == nullptr)
			parentSize = AbsoluteSize;

		Vector3 scaling(1, 1, 1);

		translation += Vector3(parentSize.X * (Position.X.Scale - 0.5f), parentSize.Y * (-Position.Y.Scale + 0.5f));

		if (parentSize.X != 0)
			scaling.X = 1 / parentSize.X;
		
		if (parentSize.Y != 0)
			scaling.Y = 1 / parentSize.Y;

		Vector3 offset = -2 * AnchorPoint.Calculate(Vector3(), AbsoluteSize).Scale(1, -1, 1);
		Vector3 rotationOffset = -2 * RotationAnchor.Calculate(Vector3(), AbsoluteSize).Scale(1, -1, 1);

		Transformation = Matrix3().Scale(scaling) * Matrix3(translation) * Matrix3(offset - rotationOffset) * Matrix3().RotateRoll(Rotation) * Matrix3(rotationOffset) * Matrix3().Scale(scale);

		if (parent != nullptr)
			Transformation = parent->GetTransformation() * Transformation;

		Transformation = Transformation;

		InverseTransformation.Invert(Transformation);

		LastSize = Size;
		LastPosition = Position;
	}

	void DeviceTransform::Draw(std::shared_ptr<Object> object, bool updateStencils)
	{
		for (int i = 0; i < object->GetChildren(); ++i)
		{
			std::shared_ptr<Object> child = object->Get(i)->Cast<Object>();

			if (child->IsA<DeviceTransform>())
			{
				if (child->Cast<DeviceTransform>()->Visible && child->GetComponent<DeviceTransform>() == This.lock())
				{
					child->Cast<DeviceTransform>()->Draw();

					Draw(child, updateStencils);
				}
			}
			else if (child->IsA<ScreenCanvas>())
			{
				if (child->Cast<ScreenCanvas>()->Visible)
				{
					if (!updateStencils && child->GetComponent<DeviceTransform>() == This.lock())
						child->Cast<ScreenCanvas>()->Draw();

					Draw(child, updateStencils);
				}
			}
			else if (child->IsA<TextCanvas>())
			{
				if (child->Cast<TextCanvas>()->Visible)
				{
					if (!updateStencils && child->GetComponent<DeviceTransform>() == This.lock())
						child->Cast<TextCanvas>()->Draw();
				
					Draw(child, updateStencils);
				}
			}
			else if (child->IsA<CanvasStencil>())
			{
				if (updateStencils && child->Cast<CanvasStencil>()->Enabled && child->GetComponent<DeviceTransform>() == This.lock())
					child->Cast<CanvasStencil>()->Draw();

				Draw(child, updateStencils);
			}
			else
				Draw(child, updateStencils);
		}
	}
}