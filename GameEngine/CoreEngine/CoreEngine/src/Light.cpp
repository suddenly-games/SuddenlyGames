#include "Light.h"

extern "C" {
#include <math.h>
}

#include "Graphics.h"
#include "Textures.h"
#include "Camera.h"
#include "FrameBuffer.h"

namespace GraphicsEngine
{
	float Light::ComputeRadius(float a, float b, float c, float value)
	{
		return (
			-b + sqrtf(
				b * b / value - 4 * a * (c - 1 / value)
			)
		) / (2 * a);
	}

	void Light::RecomputeRadius()
	{
		float a = Attenuation.Z;
		float b = Attenuation.Y;
		float c = Attenuation.X;

		ComputedRadius = ComputeRadius(a, b, c, 10 / 255.0f);

		if (Type == LightType::Spot)
		{
			Direction.Normalize();

			if (Direction == Vector3(0, 0, -1))
				ShadowMapTransformation = Matrix3(Position);
			else if (Direction == Vector3(0, 0, 1))
				ShadowMapTransformation = Matrix3(Position) * Matrix3().RotateYaw(PI / 2);
			else
				ShadowMapTransformation = Matrix3(Position) * Matrix3().RotateYaw(atan2f(Direction.X, Direction.Z)) * Matrix3().RotatePitch(acosf(Direction.Y));
		}
		else
			ShadowMapTransformation = Matrix3(Position);

		ShadowMapInverseTransformation = ShadowMapTransformation;
		ShadowMapInverseTransformation.Inverse();
	}

	Dimensions Light::GetShadowMapSize() const
	{
		return ShadowMapSize;
	}

	Aabb Light::GetBoundingBox() const
	{
		switch (Type)
		{
		case LightType::Directional:
			return Aabb(Vector3(-1e30f, -1e30f, -1e30f), Vector3(1e30f, 1e30f, 1e30f));

		case LightType::Point:
			return Aabb(Position + Vector3(-ComputedRadius, -ComputedRadius, -ComputedRadius), Position + Vector3(ComputedRadius, ComputedRadius, ComputedRadius));

		case LightType::Spot:
			if (OuterRadius > PI / 4 + 0.001f)
				return Aabb(Position + Vector3(-ComputedRadius, -ComputedRadius, -ComputedRadius), Position + Vector3(ComputedRadius, ComputedRadius, ComputedRadius));
			else
				return ComputeSpotlightBoundingBox();
		}

		return Aabb();
	}

	Aabb Light::ComputeSpotlightBoundingBox() const
	{
		Vector3 minCorner = Position;
		Vector3 maxCorner = Position;

		Vector3 right = ShadowMapTransformation.RightVector();
		Vector3 up = ShadowMapTransformation.UpVector();
		Vector3 front = ShadowMapTransformation.FrontVector();

		Vector3 corners[4] = {
			Position + ComputedRadius * ( right + up + front),
			Position + ComputedRadius * ( right + up - front),
			Position + ComputedRadius * (-right + up + front),
			Position + ComputedRadius * (-right + up - front)
		};

		for (int i = 0; i < 4; ++i)
		{
			if (corners[i].X < minCorner.X)
				minCorner.X = corners[i].X;

			if (corners[i].Y < minCorner.Y)
				minCorner.Y = corners[i].Y;

			if (corners[i].Z < minCorner.Z)
				minCorner.Z = corners[i].Z;

			if (corners[i].X > maxCorner.X)
				maxCorner.X = corners[i].X;

			if (corners[i].Y > maxCorner.Y)
				maxCorner.Y = corners[i].Y;

			if (corners[i].Z > maxCorner.Z)
				maxCorner.Z = corners[i].Z;
		}

		return Aabb(minCorner, maxCorner);
	}

	const Matrix3& Light::GetShadowMapTransformation() const
	{
		return ShadowMapTransformation;
	}

	const Matrix3& Light::GetShadowMapInverseTransformation() const
	{
		return ShadowMapInverseTransformation;
	}

	void Light::SetShadowsEnabled(bool enabled, int width, int height)
	{
		ShadowsEnabled = enabled;
		ShadowMapSize = Dimensions(unsigned int(width), unsigned int(height));

		//if (enabled)
		//{
		//	GenBuffer(RightMap, width, height);
		//	GenBuffer(LeftMap, width, height);
		//	GenBuffer(TopMap, width, height);
		//	GenBuffer(BottomMap, width, height);
		//	GenBuffer(FrontMap, width, height);
		//	GenBuffer(BackMap, width, height);
		//}
		//else
		//{
		//	DeleteBuffer(RightMap);
		//	DeleteBuffer(LeftMap);
		//	DeleteBuffer(TopMap);
		//	DeleteBuffer(BottomMap);
		//	DeleteBuffer(FrontMap);
		//	DeleteBuffer(BackMap);
		//}
	}

	bool Light::AreShadowsEnabled() const
	{
		return ShadowsEnabled;
	}

	void Light::GenBuffer(std::shared_ptr<FrameBuffer>& map, int width, int height)
	{
		if (map != nullptr)
		{
			if (map->GetWidth() != width || map->GetHeight() != height)
				map.reset();
			else
				return;
		}

		map = FrameBuffer::Create(width, height, Textures::Create(width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_R32F, GL_RED));

		FrameBuffer::Detatch();
	}

	void Light::DeleteBuffer(std::shared_ptr<FrameBuffer>& map)
	{
		map.reset();
	}

	std::shared_ptr<FrameBuffer> Light::GetShadowMap(LightDirection map) const
	{
		if (!ShadowsEnabled)
			return nullptr;

		switch (map)
		{
		case LightDirection::Right:

			return RightMap.lock();
		case LightDirection::Left:

			return LeftMap.lock();
		case LightDirection::Top:

			return TopMap.lock();
		case LightDirection::Bottom:

			return BottomMap.lock();
		case LightDirection::Front:

			return FrontMap.lock();
		case LightDirection::Back:

			return BackMap.lock();
		}

		return nullptr;
	}
}
