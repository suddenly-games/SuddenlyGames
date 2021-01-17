#include "LuaSource.h"

#include <fstream>

namespace Engine
{
	const std::string& LuaSource::GetPath() const
	{
		return FileName;
	}

	void LuaSource::LoadSource(const std::string& path)
	{
		std::fstream file(path, std::ios_base::in);

		if (!file.good() || !file.is_open())
			throw "failed to load lua script: '" + path + "'";

		char buffer[0xFFFF] = {};
		Source = "";

		while (!file.eof())
		{
			file.read(buffer, 0xFFFF);

			Source.append(buffer, int(file.gcount()));
		}

		++Version;
	}

	void LuaSource::SaveSource(const std::string& path)
	{
		std::ofstream file(path, std::ios_base::out);

		if (!file.good() || !file.is_open())
			throw "failed to open lua script: '" + path + "'";

		file << Source;

		file.flush();
		file.close();
	}

	void LuaSource::SetSource(const std::string& source)
	{
		Source = source;

		++Version;
	}

	const std::string& LuaSource::GetSource() const
	{
		return Source;
	}

	int LuaSource::GetVersion() const
	{
		return Version;
	}
}
