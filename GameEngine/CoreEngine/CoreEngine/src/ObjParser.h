#pragma once

#include <vector>

#include "Vector3.h"
#include "RGBA.h"

struct Vertex
{
	int Position, Normal, Color, UV;

	Vertex(int position = -1, int uv = -1, int normal = -1, int color = -1) : Position(position), UV(uv), Normal(normal), Color(color) {}
};

struct Face
{
	Vertex Vertices[3];

	std::string Data[3];
};

struct ParseModeEnum
{
	enum Enum
	{
		Seek,
		Position,
		UVs,
		Normal,
		Face,
		Comment,

		Error
	};
};

typedef ParseModeEnum::Enum ParseMode;
typedef std::vector<Face> FaceVector;
typedef std::vector<Vector3> VertexVector;
typedef std::vector<RGBA> ColorVector;
typedef std::vector<char> String;

class ObjParser
{
public:
	FaceVector Faces;
	VertexVector Vertices;
	VertexVector UVs;
	VertexVector Normals;
	ColorVector Colors;

	void Parse(const std::string& filePath = std::string(""));

private:
	ParseMode Mode;
	String Token;
	int TokenNumber;
	Vector3 Vector;
	Face Polygon;

	std::string Output(const std::string& filePath, int lineNumber, const std::string& error);
	void EvaluateToken();
	ParseMode SelectMode(const std::string& token);
	void ReadVertex(const std::string& token, Vertex& vertex);
	bool ReadIndex(const std::string& numberToken, int numberTokenCount, Vertex& vertex);
	bool ReadVector(const std::string& token, int tokenNumber, Vector3& vector);
	bool ReadVector2(const std::string& token, int tokenNumber, Vector3& vector);
	bool ReadToken(char character, String& token);
};
