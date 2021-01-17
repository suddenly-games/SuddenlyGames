#pragma once

#include <vector>

#include "Vector3.h"
#include "Constants.h"
#include "Object.h"
#include "AabbTree.h"
#include "Dimensions.h"

namespace GraphicsEngine
{
	class Camera;
	class FrameBuffer;

	class Light : public Engine::Object
	{
	public:
		struct LightTypeEnum
		{
			enum LightType
			{
				Directional,
				Point,
				Spot
			};
		};

		struct LightDirectionEnum
		{
			enum LightDirection
			{
				Right,
				Left,
				Top,
				Bottom,
				Front,
				Back
			};
		};

		typedef LightTypeEnum::LightType LightType;
		typedef LightDirectionEnum::LightDirection LightDirection;

		virtual ~Light() {}

		void Initialize() {}
		void Update(float) {}

		bool ShadowDebugView = false;
		bool Enabled = true;
		float Brightness = 1;
		Vector3 Attenuation;
		Vector3 Position;
		Vector3 Direction;
		RGBA Diffuse = 0xFFFFFFFF;
		RGBA Specular = 0xFFFFFFFF;
		RGBA Ambient = 0xFFFFFFFF;
		float InnerRadius = PI / 5;
		float OuterRadius = PI / 3;
		int SpotlightFalloff = 1;
		LightType Type = LightType::Directional;

		float GetRadius() const { return ComputedRadius; }
		void RecomputeRadius();
		Dimensions GetShadowMapSize() const;
		void SetShadowsEnabled(bool enabled, int width = 128, int height = 128);
		bool AreShadowsEnabled() const;
		std::shared_ptr<FrameBuffer> GetShadowMap(LightDirection map) const;
		Aabb GetBoundingBox() const;
		Aabb ComputeSpotlightBoundingBox() const;
		const Matrix3& GetShadowMapTransformation() const;
		const Matrix3& GetShadowMapInverseTransformation() const;

		static float ComputeRadius(float a, float b, float c, float value);

	private:
		bool ShadowsEnabled = false;
		float ComputedRadius = 0;
		Matrix3 ShadowMapTransformation;
		Matrix3 ShadowMapInverseTransformation;

		std::weak_ptr<FrameBuffer> RightMap;
		std::weak_ptr<FrameBuffer> LeftMap;
		std::weak_ptr<FrameBuffer> TopMap;
		std::weak_ptr<FrameBuffer> BottomMap;
		std::weak_ptr<FrameBuffer> FrontMap;
		std::weak_ptr<FrameBuffer> BackMap;

		Dimensions ShadowMapSize;

		void GenBuffer(std::shared_ptr<FrameBuffer>& map, int width, int height);
		void DeleteBuffer(std::shared_ptr<FrameBuffer>& map);

		Instantiable;

		Inherits_Class(Object);

		Reflected(Light);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Light, Object);

	Declare_Enum(LightType);
}

namespace Enum
{
	typedef GraphicsEngine::Light::LightType LightType;
	typedef GraphicsEngine::Light::LightDirection LightDirection;
}