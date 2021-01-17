#include "Materials.h"

#include "Material.h"

namespace GraphicsEngine
{
	std::shared_ptr<Material> NewMaterial(std::shared_ptr<Material> material, const std::string& name, unsigned int diffuse, unsigned int specular, unsigned int ambient, unsigned int emissive, int shininess)
	{
		material->Name = name;
		material->Diffuse = diffuse;
		material->Specular = specular;
		material->Ambient = ambient;
		material->Emission = emissive;
		material->Shininess = shininess;

		return material;
	}

	void Materials::Initialize()
	{
		std::shared_ptr<ObjectBase> materials = This.lock();

		NewMaterial(Engine::Create<Material>(), "Default",    0x808080FF, 0x808080FF, 0x808080FF, 0x00000000,   1)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Glow",       0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF,   1)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Reflective", 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 255)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Black",      0x00000000, 0x00000000, 0x00000000, 0x00000000,   1)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Metallic",   0x808080FF, 0xFFFFFFFF, 0x666666FF, 0x00000000,   5)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Highlight",  0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 255)->SetParent(materials);
		NewMaterial(Engine::Create<Material>(), "Custom",     0x101010FF, 0xFFFFFFFF, 0x101010FF, 0x7F7F7F7F, 255)->SetParent(materials);
	}
}


