#include "Camera.h"

extern "C" {
#include <math.h>
}

#include "Constants.h"

namespace GraphicsEngine
{
	void Camera::SetTransformation(const Matrix3& newTransformation)
	{
		Transformation = newTransformation;
		InverseTransformation.Invert(Transformation);
		Projection = PerspectiveProjection * InverseTransformation;

		CalculateFrustum();
	}

	void Camera::SetProperties(float newFieldOfView, float newAspectRatio, float near, float far)
	{
		if (far < 0)
			far *= -1;

		if (far < near)
			far = near + 1e-5f;

		ProjectionPlane = near;
		NearPlane = near;
		FarPlane = far;
		AspectRatio = newAspectRatio;
		FieldOfView = newFieldOfView;
		Height = 2 * near * (float)tan(FieldOfView / 2);
		Width = AspectRatio * Height;

		if (FieldOfView >= PI)
			FieldOfView = PI - 1e-5f;
		else if (FieldOfView <= 1e-5f)
			FieldOfView = 1e-5f;

		CalculateProjectionMatrix();

		CalculateFrustum();
	}

	void Camera::SetProperties(float newWidth, float newHeight, float newProjectionPlane, float near, float far)
	{
		if (far < 0)
			far *= -1;

		if (far < near)
			far = near + 1e-5f;

		Width = newWidth;
		Height = newHeight;
		ProjectionPlane = newProjectionPlane;
		NearPlane = near;
		FarPlane = far;
		AspectRatio = Width / Height;
		FieldOfView = 2 * (float)atan(Height / (2 * ProjectionPlane));

		if (ProjectionPlane < near)
			ProjectionPlane = near;
		else if (ProjectionPlane > far)
			ProjectionPlane = far;

		CalculateProjectionMatrix();

		CalculateFrustum();
	}

	void Camera::CalculateProjectionMatrix()
	{
		PerspectiveProjection = Matrix3().Projection(ProjectionPlane, NearPlane, FarPlane, Width, Height);
		Projection = PerspectiveProjection * InverseTransformation;
	}

	const Matrix3& Camera::GetTransformation() const
	{
		return Transformation;
	}

	const Matrix3& Camera::GetTransformationInverse() const
	{
		return InverseTransformation;
	}

	const Matrix3& Camera::GetProjectionMatrix() const
	{
		return PerspectiveProjection;
	}

	const Matrix3& Camera::GetProjection() const
	{
		return Projection;
	}

	Vector3 Camera::GetDimensions() const
	{
		return Vector3(Width, Height);
	}

	float Camera::GetProjectionPlane() const
	{
		return ProjectionPlane;
	}

	float Camera::GetNearPlane() const
	{
		return NearPlane;
	}

	float Camera::GetFarPlane() const
	{
		return FarPlane;
	}

	float Camera::GetAspectRatio() const
	{
		return AspectRatio;
	}

	float Camera::GetFieldOfView() const
	{
		return FieldOfView;
	}

	void Camera::CalculateFrustum()
	{
		Vector3 translation = Transformation.Translation();
		Vector3 right = Transformation.RightVector();
		Vector3 up = Transformation.UpVector();
		Vector3 front = Transformation.FrontVector();

		Vector3 frontVec = -front;
		Vector3 rightVec = Width * 0.5f * right;
		Vector3 upVec = Height * 0.5f * up;

		Vector3 direction1 = frontVec - rightVec - upVec;
		Vector3 direction2 = frontVec + rightVec - upVec;
		Vector3 direction3 = frontVec - rightVec + upVec;
		Vector3 direction4 = frontVec + rightVec + upVec;

		CameraFrustum.Planes[0].Set(translation - NearPlane * front, -front);
		CameraFrustum.Planes[1].Set(translation, -direction3.Cross(direction1));
		CameraFrustum.Planes[2].Set(translation, -direction2.Cross(direction4));
		CameraFrustum.Planes[3].Set(translation, -direction1.Cross(direction2));
		CameraFrustum.Planes[4].Set(translation, -direction4.Cross(direction3));
		CameraFrustum.Planes[5].Set(translation - FarPlane * front, front);
	}

	const Frustum& Camera::GetFrustum() const
	{
		return CameraFrustum;
	}
}
