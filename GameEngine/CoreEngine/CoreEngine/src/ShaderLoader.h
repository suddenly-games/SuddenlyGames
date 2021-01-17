#pragma once

#include <vector>

#include "Shader.h"

class ShaderLoader
{
public:
	typedef std::vector<Shader*> ShaderVector;

	ShaderVector Shaders;

	ShaderLoader();
};

class ShaderCleaner
{
public:
	ShaderCleaner(ShaderLoader* loader);
};
