#include "JsonParser.h"

#include <fstream>
#include <sstream>

JsonParser::JsonParser(const std::string& data, bool isPath)
{
	if (isPath)
	{
		FilePath = data;

		std::ifstream file(data, std::ios_base::in);

		if (!file.is_open() || !file.good())
			throw std::string("cannot open file: '") + data + "'";

		while (!file.eof())
		{
			InputBuffer.resize(InputBuffer.size() + 4096);

			int startIndex = int(InputBuffer.size()) - 4096;

			file.read(InputBuffer.data() + InputBuffer.size() - 4096, 4096);

			int count = int(file.gcount());

			if (startIndex + count < int(InputBuffer.size()))
				InputBuffer[startIndex + count] = 0;
		}

		Input = std::string(InputBuffer.data(), InputBuffer.size());
	}
	else
		Input = data;
}

void JsonParser::Parse()
{
	Index = 0;
	Stack.clear();

	while (!Finished())
	{
		SkipWhitespace();

		if (Finished())
			break;

		if (Input[Index] == '{')
		{
			if (Stack.size() > 0 && Stack[Stack.size() - 1].IsArray)
			{
				ReadIndex(Stack[Stack.size() - 1].Index);
				++Stack[Stack.size() - 1].Index;
			}

			Stack.push_back(StackItem{ 0, false });
			PushContainer(false);

			++Index;
		}
		else if (Input[Index] == '[')
		{
			if (Stack.size() > 0 && Stack[Stack.size() - 1].IsArray)
			{
				ReadIndex(Stack[Stack.size() - 1].Index);
				++Stack[Stack.size() - 1].Index;
			}

			Stack.push_back(StackItem{ 0, true });
			PushContainer(true);

			++Index;
		}
		else
		{
			if (Stack.size() == 0)
				Throw((std::string("unexpected symbol '") + Input[Index]) + "'");

			if (Input[Index] == '"')
			{
				if (Stack[Stack.size() - 1].IsArray)
					ReadArrayItem();
				else
					ReadDictionaryItem();
			}
			else if (Input[Index] == '\'')
			{
				if (Stack[Stack.size() - 1].IsArray)
					ReadArrayItem();
				else
					ReadDictionaryItem();
			}
			else if (((Input[Index] >= '0' && Input[Index] <= '9') || Input[Index] == '.' || Input[Index] == '-') && Stack[Stack.size() - 1].IsArray)
				ReadArrayNumber();
			else if (Input[Index] == 't' || Input[Index] == 'T')
			{
			
				if (Stack[Stack.size() - 1].IsArray)
					ReadArrayItem();
				else
					ReadDictionaryItem();
				//ReadTrue();
				//
				//CheckForComma();
			}
			else if (Input[Index] == 'f' || Input[Index] == 'F')
			{
			
				if (Stack[Stack.size() - 1].IsArray)
					ReadArrayItem();
				else
					ReadDictionaryItem();
				//ReadFalse();
				//
				//CheckForComma();
			}
			else if (Input[Index] == '}')
			{
				if (Stack[Stack.size() - 1].IsArray)
					Throw((std::string("unexpected symbol '") + Input[Index]) + "'");

				PopContainer();
				Stack.pop_back();

				++Index;

				CheckForComma();
			}
			else if (Input[Index] == ']')
			{
				if (!Stack[Stack.size() - 1].IsArray)
					Throw((std::string("unexpected symbol '") + Input[Index]) + "'");

				PopContainer();
				Stack.pop_back();

				++Index;

				CheckForComma();
			}
			else
				Throw((std::string("unexpected symbol '") + Input[Index]) + "'");
		}
	}

	if (Stack.size() > 0)
		Throw("unclosed stack");
}

void JsonParser::SkipWhitespace()
{
	while (!Finished() && (
		Input[Index] == ' ' ||
		Input[Index] == '\t' ||
		Input[Index] == '\r' ||
		Input[Index] == '\n'
	))
	{
		if (Input[Index] == '\n')
		{
			LineStart = Index + 1;

			++LineNumber;
		}
		else if (Input[Index] == '\r')
			LineStart = Index + 1;

		++Index;
	}
}

void JsonParser::ReadArrayItem()
{
	ReadIndex(Stack[Stack.size() - 1].Index);

	++Stack[Stack.size() - 1].Index;

	if (Input[Index] == '"' || Input[Index] == '\'')
		ReadString(ReadRawString());
	else if (Input[Index] == 't' || Input[Index] == 'T')
		ReadTrue();
	else if (Input[Index] == 'f' || Input[Index] == 'F')
		ReadFalse();
	else
		ReadRawNumber();

	CheckForComma();
}

void JsonParser::ReadDictionaryItem()
{
	ReadKey(ReadRawString());

	SkipWhitespace();

	if (Finished() || Input[Index] != ':')
		Throw("expected ':'");

	++Index;

	SkipWhitespace();

	if (Finished() || Input[Index] == '{' || Input[Index] == '[')
		return;

	if (Input[Index] == '"' || Input[Index] == '\'')
		ReadString(ReadRawString());
	else if (Input[Index] == 't' || Input[Index] == 'T')
		ReadTrue();
	else if (Input[Index] == 'f' || Input[Index] == 'F')
		ReadFalse();
	else
		ReadRawNumber();

	CheckForComma();
}

void JsonParser::ReadArrayNumber()
{
	ReadIndex(Stack[Stack.size() - 1].Index);

	++Stack[Stack.size() - 1].Index;

	ReadRawNumber();

	CheckForComma();
}

void JsonParser::ReadRawNumber()
{
	int start = Index;
	bool leadingInt = false;

	if (Input[Index] == '-')
		++Index;

	while (!Finished() && Input[Index] >= '0' && Input[Index] <= '9')
	{
		leadingInt = true;

		++Index;
	}

	if (!leadingInt && Input[Index] != '.')
		Throw("malformed number: missing integer digits");

	if (Finished() || (Input[Index] != 'e' && Input[Index] != 'E' && Input[Index] != '.'))
	{
		FinishNumber(start);

		return;
	}

	bool fractionalComponent = false;
	bool hasDot = Input[Index] == '.';

	if (hasDot)
	{
		++Index;

		while (!Finished() && Input[Index] >= '0' && Input[Index] <= '9')
		{
			fractionalComponent = true;

			++Index;
		}

		if (!fractionalComponent)
			Throw("malformed number: missing fraction");
	}

	if (!Finished() && Input[Index] == 'e' || Input[Index] == 'E')
	{
		if ((hasDot && !fractionalComponent) || (!hasDot && !leadingInt))
			Throw("malformed number: missing digits before exponent");

		++Index;

		bool exponent = false;

		while (!Finished() && Input[Index] >= '0' && Input[Index] <= '9')
		{
			exponent = true;

			++Index;
		}

		if (!exponent)
			Throw("malformed number: missing exponent");
	}

	FinishNumber(start);
}

void JsonParser::ReadTrue()
{
	++Index;

	if (Finished() || (Input[Index] != 'r' && Input[Index] != 'R'))
		Throw("invalid data");

	++Index;

	if (Finished() || (Input[Index] != 'u' && Input[Index] != 'U'))
		Throw("invalid data");

	++Index;

	if (Finished() || (Input[Index] != 'e' && Input[Index] != 'E'))
		Throw("invalid data");

	++Index;

	ReadBool(true);
}

void JsonParser::ReadFalse()
{
	++Index;

	if (Finished() || (Input[Index] != 'a' && Input[Index] != 'A'))
		Throw("invalid data");

	++Index;

	if (Finished() || (Input[Index] != 'l' && Input[Index] != 'L'))
		Throw("invalid data");

	++Index;

	if (Finished() || (Input[Index] != 's' && Input[Index] != 'S'))
		Throw("invalid data");

	++Index;

	if (Finished() || (Input[Index] != 'e' && Input[Index] != 'E'))
		Throw("invalid data");

	++Index;

	ReadBool(false);
}

std::string JsonParser::ReadRawString()
{
	int start = Index + 1;

	bool doubleQuotes = Input[Index] == '"';
	bool escaped = false;

	++Index;

	while (!Finished() && !(!escaped && ((doubleQuotes && Input[Index] == '"') || (!doubleQuotes && Input[Index] == '\''))))
	{
		if (escaped)
			escaped = false;
		else if (Input[Index] == '\\')
			escaped = true;

		++Index;
	}

	if (Finished())
		throw "unclosed string";

	std::string text(Input.data() + start, Index - start);

	++Index;

	return text;
}

void JsonParser::FinishNumber(int start)
{
	float number = float(atof(Input.data() + start));
	char type = 0;

	if (!Finished() && (Input[Index] >= 'a' && Input[Index] <= 'z') || (Input[Index] >= 'A' && Input[Index] <= 'Z'))
	{
		type = Input[Index];

		++Index;
	}

	ReadNumber(number, type);
}

void JsonParser::CheckForComma()
{
	SkipWhitespace();

	if (!Finished() && Input[Index] != ',' && Input[Index] != ']' && Input[Index] != '}')
		Throw("expected ','");
	
	if (Input[Index] == ',')
		++Index;
}

bool JsonParser::Finished()
{
	return Index >= int(Input.size()) || Input[Index] == 0;
}

void JsonParser::Throw(const std::string& error)
{
	std::stringstream message;
	message << FilePath << " (" << LineNumber << ", " << (Index - LineStart) << "): " << error;

	throw message.str();
}