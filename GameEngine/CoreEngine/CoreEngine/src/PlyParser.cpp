#include "PlyParser.h"

#include <fstream>

const int PlyParser::TypeSizes[Type::Unknown] = { 1, 1, 2, 2, 4, 4, 4, 8 };

void PlyParser::Parse(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios_base::in | std::ios::binary);

	if (!file.is_open() || !file.good())
		throw std::string("cannot open file: '") + filePath + "'";

	std::string token;
	bool headerEnded = false;
	int line = 0;

	HeaderTokens.push_back(StringVector());

	while (!file.eof() && !headerEnded)
	{
		char character;

		file >> std::noskipws >> character;

		if ((character == ' ' || character == '\n' || character == '\r') && token.size() != 0)
		{
			HeaderTokens.back().push_back(token);

			if (character == '\n' || character == '\r')
			{
				HeaderTokens.push_back(StringVector());

				++line;
			}

			if (token == "end_header")
				headerEnded = true;

			token.clear();
		}
		else
			token.push_back(character);
	}

	ProcessHeader();

	if (FileFormat == Format::Ascii)
		ParseAscii(file);
	else
		ParseBinary(file);
}

void PlyParser::ProcessHeader()
{
	TypeDictionary types = {
		{ "char", Type::Char },
		{ "uchar", Type::UChar },
		{ "short", Type::Short },
		{ "ushort", Type::UShort },
		{ "int", Type::Int },
		{ "uint", Type::UInt },
		{ "float", Type::Float },
		{ "double", Type::Double },
		{ "int8", Type::Char },
		{ "uint8", Type::UChar },
		{ "int16", Type::Short },
		{ "uint16", Type::UShort },
		{ "int32", Type::Int },
		{ "uint32", Type::UInt },
		{ "float32", Type::Float },
		{ "float64", Type::Double }
	};

	for (int line = 0; line < int(HeaderTokens.size()); ++line)
	{
		if (HeaderTokens[line].size() < 2 || HeaderTokens[line][0] == "ply" || HeaderTokens[line][0] == "comment")
			continue;

		const StringVector& tokens = HeaderTokens[line];

		if (tokens[0] == "format")
		{
			if (tokens[1] == "ascii")
				FileFormat = Format::Ascii;
			else if (tokens[1] == "binary_little_endian")
				FileFormat = Format::LittleEndian;
			else if (tokens[1] == "binary_big_endian")
				FileFormat = Format::BigEndian;
		}
		else if (tokens[0] == "element" && tokens.size() == 3)
		{
			Element element;

			element.Name = tokens[1];
			element.Count = std::atoi(tokens[2].c_str());

			Definitions.push_back(element);
		}
		else if (tokens[0] == "property")
		{
			if (tokens.size() == 3 && tokens[1] != "list")
			{
				Property property;

				property.Name = tokens[2];
				property.DataType = types[tokens[1]];

				Definitions.back().Properties.push_back(property);
			}
			else if (tokens.size() == 5 && tokens[1] == "list")
			{
				Property property;

				property.Name = tokens[4];
				property.DataType = types[tokens[3]];
				property.ListCounterType = types[tokens[2]];
				property.IsArray = true;

				Definitions.back().Properties.push_back(property);
			}
		}
	}
}

void PlyParser::ParseAscii(std::ifstream& file)
{
	throw "ascii types not supported yet";
}

void PlyParser::ParseBinary(std::ifstream& file)
{
	{
		int data = 0xFF;
		Format architecture = Format::BigEndian;

		if (*reinterpret_cast<char*>(&data) != 0xFF)
			architecture = Format::LittleEndian;

		FlipEndian = FileFormat != architecture;
	}

	for (int i = 0; i < int(Definitions.size()); ++i)
	{
		auto& element = Definitions[i];

		for (int count = 0; count < element.Count; ++count)
		{
			ElementData elementValue;
			std::string data;

			elementValue.Element = &element;

			int index = 0;

			for (int j = 0; j < int(element.Properties.size()); ++j)
			{
				auto& property = element.Properties[j];

				Value value;
				int dataCount = 1;

				value.Property = &property;

				if (property.IsArray)
				{
					data.clear();

					for (int i = 0; i < TypeSizes[property.ListCounterType]; ++i)
					{
						char character = 0;

						file >> std::noskipws >> character;

						data.push_back(character);
					}

					index = 0;
					dataCount = ReadData(data, index, property.ListCounterType);
				}

				data.clear();

				for (int i = 0; i < TypeSizes[property.DataType] * dataCount; ++i)
				{
					char character = 0;

					file >> std::noskipws >> character;

					data.push_back(character);
				}

				index = 0;
				for (int k = 0; k < dataCount; ++k)
					ReadData(data, value.Data, index, property.DataType);

				elementValue.Data.push_back(value);
			}

			MeshData.push_back(elementValue);
		}
	}
}

void PlyParser::ReadData(const std::string& source, std::string& destination, int& index, Type type)
{
	int size = TypeSizes[type];

	if (FlipEndian)
	{
		for (int i = size - 1; i >= 0; --i)
			destination.push_back(source[index + i]);
	}
	else
	{
		for (int i = 0; i < size; ++i)
			destination.push_back(source[index + i]);
	}

	index += size;
}

namespace
{
	template <typename T>
	inline int cast(char* data)
	{
		return int(*reinterpret_cast<T*>(data));
	}
}

int PlyParser::ReadData(const std::string& source, int& index, Type type)
{
	int size = TypeSizes[type];
	char buffer[8] = {};

	if (FlipEndian)
	{
		for (int i = 0; i < size; ++i)
			buffer[size - i - 1] = source[index + i];
	}
	else
	{
		for (int i = 0; i < size; ++i)
			buffer[i] = source[index + i];
	}

	index += size;

	switch (type)
	{
	case Type::Char: return cast<signed char>(buffer);
	case Type::UChar: return cast<unsigned char>(buffer);
	case Type::Short: return cast<short>(buffer);
	case Type::UShort: return cast<unsigned short>(buffer);
	case Type::Int: return cast<int>(buffer);
	case Type::UInt: return cast<unsigned int>(buffer);
	case Type::Float: return cast<float>(buffer);
	case Type::Double: return cast<double>(buffer);
	}

	return 0;
}