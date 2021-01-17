#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <map>

class PlyParser
{
public:
	struct FormatEnum
	{
		enum Format
		{
			Ascii,
			LittleEndian,
			BigEndian,
			Unknown
		};
	};

	struct TypeEnum
	{
		enum Type
		{
			Char,
			UChar,
			Short,
			UShort,
			Int,
			UInt,
			Float,
			Double,
			Unknown
		};
	};

	typedef FormatEnum::Format Format;
	typedef TypeEnum::Type Type;

	struct Property
	{
		std::string Name;
		Type DataType = Type::Unknown;
		Type ListCounterType = Type::Unknown;
		bool IsArray = false;
	};

	struct Element
	{
		typedef std::vector<Property> PropertyVector;

		std::string Name;
		int Count = 0;
		PropertyVector Properties;
	};

	struct Value
	{
		Property* Property = nullptr;
		std::string Data;

		inline int Size() const
		{
			return TypeSizes[Property->DataType];
		}

		inline int Offset(int i) const
		{
			return Size() * i;
		}

		inline int Count() const
		{
			if (!Property->IsArray)
				return 1;
			else
				return int(Data.size()) / Size();
		}

		template <typename T>
		inline T cast(const char* data) const
		{
			return T(*reinterpret_cast<const T*>(data));
		}

		template <typename T>
		T convert(const char* data) const
		{
			switch (Property->DataType)
			{
			case Type::Char: return T(cast<signed char>(data));
			case Type::UChar: return T(cast<unsigned char>(data));
			case Type::Short: return T(cast<short>(data));
			case Type::UShort: return T(cast<unsigned short>(data));
			case Type::Int: return T(cast<int>(data));
			case Type::UInt: return T(cast<unsigned int>(data));
			case Type::Float: return T(cast<float>(data));
			case Type::Double: return T(cast<double>(data));
			}

			return T();
		}

		template<typename T>
		T get(int i = 0) const { return T(); }

		template<>
		inline signed char get<signed char>(int i) const { return convert<signed char>(Data.c_str() + Offset(i)); }

		template<>
		inline unsigned char get<unsigned char>(int i) const { return convert<unsigned char>(Data.c_str() + Offset(i)); }

		template<>
		short get<short>(int i) const { return convert<short>(Data.c_str() + Offset(i)); }

		template<>
		unsigned short get<unsigned short>(int i) const { return convert<unsigned short>(Data.c_str() + Offset(i)); }

		template<>
		int get<int>(int i) const { return convert<int>(Data.c_str() + Offset(i)); }

		template<>
		unsigned int get<unsigned int>(int i) const { return convert<unsigned int>(Data.c_str() + Offset(i)); }

		template<>
		float get<float>(int i) const { return convert<float>(Data.c_str() + Offset(i)); }

		template<>
		double get<double>(int i) const { return convert<double>(Data.c_str() + Offset(i)); }
	};

	struct ElementData
	{
		typedef std::vector<Value> DataVector;

		Element* Element = nullptr;
		DataVector Data;
	};

	typedef std::vector<Element> ElementVector;
	typedef std::vector<ElementData> ElementDataVector;
	typedef std::vector<std::string> StringVector;
	typedef std::vector<StringVector> StringCollectionVector;

	ElementVector Definitions;
	ElementDataVector MeshData;

	void Parse(const std::string& filePath = std::string(""));

private:

	typedef std::map<std::string, Type> TypeDictionary;

	static const int TypeSizes[Type::Unknown];

	StringCollectionVector HeaderTokens;
	Format FileFormat = Format::Unknown;
	bool FlipEndian = false;

	void ProcessHeader();
	void ParseAscii(std::ifstream& file);
	void ParseBinary(std::ifstream& file);
	void ReadData(const std::string& source, std::string& destination, int& index, Type type);
	int ReadData(const std::string& source, int& index, Type type);
};