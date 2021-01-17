#pragma once

#include "Object.h"

#include "MeshData.h"

class MeshLoader;

namespace Engine
{
	class ModelAsset : public Object
	{
	public:
		void Initialize() {}
		void Update(float) {}
		~ModelAsset();

		int GetMeshID() const;
		bool IsCore() const;
		bool IsLoaded() const;
		bool IsDying() const { return Dying; }
		Vector3 GetMinimumCorner() const;
		Vector3 GetMaximumCorner() const;
		Vector3 GetCenter() const;
		Vector3 GetSize() const;
		void Load(const std::string& path, Enum::VertexMode mode = Enum::VertexMode::Seperate);
		void Unload();
		void Release();

		friend class MeshLoader;

	private:
		int MeshID = -1;
		bool Dying = false;

		Instantiable;

		Inherits_Class(Object);

		Reflected(ModelAsset);
	};

	//Class_Inherits(ModelAsset, Object);
}
