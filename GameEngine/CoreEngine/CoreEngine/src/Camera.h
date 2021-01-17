#pragma once

#include "ShaderProgram.h"
#include "Matrix3.h"
#include "Object.h"
#include "Frustum.h"

namespace GraphicsEngine
{
	class Camera : public Engine::Object
	{
	public:
		virtual ~Camera() {}

		void Initialize() {}
		void Update(float) {}

		Vector3 LightDirection;

		void SetTransformation(const Matrix3& newTransformation);
		void SetProperties(float fieldOfView, float aspectRatio, float near, float far);
		void SetProperties(float width, float height, float projectionPlane, float near, float far);
		const Matrix3& GetTransformation() const;
		const Matrix3& GetTransformationInverse() const;
		const Matrix3& GetProjectionMatrix() const;
		const Matrix3& GetProjection() const;
		Vector3 GetDimensions() const;
		float GetProjectionPlane() const;
		float GetNearPlane() const;
		float GetFarPlane() const;
		float GetAspectRatio() const;
		float GetFieldOfView() const;
		const Frustum& GetFrustum() const;

	private:
		Matrix3 PerspectiveProjection;
		Matrix3 Transformation;
		Matrix3 InverseTransformation;
		Matrix3 Projection;
		Frustum CameraFrustum;

		float Width = 0, Height = 0, ProjectionPlane = 0, NearPlane = 0, FarPlane = 0, AspectRatio = 0, FieldOfView = 0;

		void CalculateProjectionMatrix();
		void CalculateFrustum();

		Instantiable;

		Inherits_Class(Object);

		Reflected(Camera);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Camera, Object);
}
