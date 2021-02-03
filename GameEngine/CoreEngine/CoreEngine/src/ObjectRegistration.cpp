#include "ObjectRegistration.h"

#include <vector>

#include "Vector3.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "RGBA.h"

#include "Object.h"
#include "GameWindow.h"
#include "LuaInput.h"
#include "Environments.h"
#include "Environment.h"
#include "Simulation.h"
#include "Transform.h"
#include "MeshLoader.h"
#include "ModelAsset.h"
#include "Material.h"
#include "Materials.h"
#include "Dimensions.h"
#include "Texture.h"
#include "Textures.h"
#include "FrameBuffer.h"
#include "Font.h"
#include "Fonts.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Scene.h"
#include "RenderOperation.h"
#include "ShadingOperation.h"
#include "GlowingSceneOperation.h"
#include "DrawSceneOperation.h"
#include "BlurOperation.h"
#include "CubeMap.h"
#include "LuaSource.h"
#include "LuaScript.h"
#include "DeviceVector.h"
#include "DeviceTransform.h"
#include "Appearance.h"
#include "ScreenCanvas.h"
#include "InterfaceDrawOperation.h"
#include "CanvasStencil.h"
#include "Aabb.h"
#include "OBB.h"
#include "Ray.h"
#include "BoundingSphere.h"
#include "Collider2D.h"
#include "ColliderAsset.h"
#include "PointMass.h"
#include "RigidBody.h"
#include "PhysicsEnvironment.h"
#include "CollisionGroup.h"
#include "Text.h"

#include "Lua.h"
#include "PageAllocator.h"

#include "PointGenerator.h"
#include "ParticleEmitter.h"

#include "DebugDraw.h"

#include "CellData.h"
#include "Voxel.h"
#include "Chunk.h"
#include "Terrain.h"
#include "TerrainMaterial.h"
#include "ChunkLoader.h"

#include "ChunkLoaderShapeEnum.h"
#include "ChunkStatusEnum.h"
#include "VertexModeEnum.h"

#define Register_Type(typeName) typeName::TypeData::Initialize();
#define Register_Class(typeName) typeName::ObjectData::Initialize();

namespace Engine
{
	void InitializeObjectTypes()
	{
		initializeCoreTypes();
		Lua::InitializeLuaTypes();

		EnumClass::Type = new EnumClass("EnumType", "The base of all types of enums.");

		EnumClass::Type->IsEnum = true;
		EnumClass::Type->IsClass = false;

		Register_Type(Vector3);
		Register_Type(Matrix3);
		Register_Type(Quaternion);
		Register_Type(RGBA);
		Register_Type(DeviceAxis);
		Register_Type(DeviceVector);

		Register_Class(ObjectBase);
		Register_Class(Object);
		Register_Enum(InputCode);
		Register_Enum(InputType);
		Register_Enum(InputState);
		Register_Class(InputBase);
		Register_Class(InputData);
		Register_Class(UserInput);
		Register_Class(GraphicsEngine::GameWindow);
		Register_Class(Environments);
		Register_Class(Environment);
		Register_Class(Simulation);
		Register_Class(Transform);
		Register_Enum(VertexMode);
		Register_Class(ModelAsset);

		Register_Type(Aabb);
		Register_Type(OBB);
		Register_Type(BoundingSphere);
		Register_Type(Ray);

		Register_Class(MeshLoader);
		Register_Class(GraphicsEngine::Material);
		Register_Class(GraphicsEngine::Materials);
		Register_Enum(SampleType);
		Register_Enum(WrapType);
		Register_Enum(InternalFormat);
		Register_Enum(Format);
		Register_Enum(DataType);
		Register_Class(GraphicsEngine::Texture);
		Register_Type(Dimensions);
		Register_Class(GraphicsEngine::Textures);
		Register_Class(GraphicsEngine::FrameBuffer);
		Register_Type(GraphicsEngine::Font::Character);
		Register_Class(GraphicsEngine::Font);
		Register_Class(GraphicsEngine::Fonts);
		Register_Class(GraphicsEngine::Camera);
		Register_Enum(LightType);
		Register_Class(GraphicsEngine::Light);
		Register_Class(GraphicsEngine::SceneObject);

		Register_Enum(ChunkLoaderShape);

		Register_Class(TerrainMaterial);
		Register_Class(Voxel);
		Register_Type(CellData);
		Register_Enum(ChunkStatus);
		Register_Class(Chunk);
		Register_Class(Terrain);
		Register_Class(ChunkLoader);

		Register_Class(GraphicsEngine::Scene);
		Register_Class(GraphicsEngine::Model);
		Register_Class(GraphicsEngine::RenderOperation);
		Register_Class(GraphicsEngine::BlurPassOperation);
		Register_Class(GraphicsEngine::BlurOperation);
		Register_Class(GraphicsEngine::DrawSceneOperation);
		Register_Class(GraphicsEngine::ShadingOperation);
		Register_Class(CubeMapTexture);
		Register_Class(CubeMapBuffer);
		Register_Enum(LuminescenceMode);
		Register_Enum(RangeFittingMode);
		Register_Class(GraphicsEngine::GlowingSceneOperation);
		Register_Enum(ScriptStatus);
		Register_Class(LuaSource);
		Register_Class(LuaScript);
		Register_Enum(Alignment);
		Register_Class(GraphicsEngine::Appearance);
		Register_Class(GraphicsEngine::ScreenCanvas);
		Register_Class(GraphicsEngine::DeviceTransform);
		Register_Class(GraphicsEngine::CanvasStencil);
		Register_Class(GraphicsEngine::InterfaceDrawOperation);
		Register_Class(GraphicsEngine::Text);

		Register_Type(VectorGenerator);
		Register_Type(NumberRange);
		Register_Type(PointGenerator);
		Register_Type(UnitVectorGenerator);
		Register_Type(ConeUnitVectorGenerator);

		Register_Enum(InteractionType);
		Register_Type(Collision);
		Register_Class(Physics::ColliderAsset);
		Register_Class(Physics::CollisionGroup);
		Register_Class(Physics::Collider2D);
		Register_Type(RayCastResults);
		Register_Class(Physics::PointMass);
		Register_Class(Physics::RigidBody);
		Register_Class(Physics::PhysicsEnvironment);

		Register_Class(GraphicsEngine::EmitterConfig);
		Register_Class(GraphicsEngine::ParticleEmitter);
		Register_Class(GraphicsEngine::ParticleConfiguration);
		Register_Class(GraphicsEngine::DirectionalParticleSpawner);
		Register_Class(GraphicsEngine::RisingParticleSpawner);
		Register_Class(GraphicsEngine::PlanarParticleSpawner);
		Register_Class(GraphicsEngine::LineParticleSpawner);
		Register_Class(GraphicsEngine::RingParticleSpawner);
		Register_Class(GraphicsEngine::ConeParticleSpawner);
		Register_Class(GraphicsEngine::GravityParticleUpdater);
		Register_Class(GraphicsEngine::DragParticleUpdater);
		Register_Class(GraphicsEngine::DampenedForceUpdater);
		Register_Class(GraphicsEngine::CubicBezierUpdater);
		Register_Class(DebugDraw);

		const ReflectionData::DataVector& types = ReflectionData::GetTypes();

		EnumType& objectType = EnumType::AddType("ObjectType");
		int objectTypeID = 0;

		for (int i = 0; i < int(types.size()); ++i)
			if (types[i]->Inherits("Object"))
				objectType.AddItem(types[i]->Name, objectTypeID++, types[i]->Description);

		objectType.InitReflection();
	}
}
