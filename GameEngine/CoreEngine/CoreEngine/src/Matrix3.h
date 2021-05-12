/*
Matrix.H
Author: 
9/16/16

contains declaration of Matrix class
*/

#pragma once

#include "Vector3.h"
#include "ObjectReflection.h"

class Matrix3
{
public:
	float Data[4][4] = { {} };

	Matrix3();
	Matrix3(float x, float y, float z);
	Matrix3(const Vector3& vector);
	Matrix3(const Vector3& position, const Vector3& right, const Vector3& up, const Vector3& front);
	Matrix3(const Vector3& position, const Vector3& direction, const Vector3& globalUp = Vector3(0, 1, 0));
	Matrix3(const Matrix3& mat)
	{
		for (int i = 0; i < 16; ++i)
			Data[i % 4][i / 4] = mat.Data[i % 4][i / 4];
	}

	Matrix3& Identity();
	Matrix3& Transpose();
	Matrix3& SetVectors(const Vector3& position, const Vector3& right, const Vector3& up, const Vector3& front);
	Matrix3& SetRight(const Vector3& vector);
	Matrix3& SetUp(const Vector3& vector);
	Matrix3& SetFront(const Vector3& vector);
	Matrix3& SetTranslation(const Vector3& vector);
	Matrix3& Translate(float x, float y, float z);
	Matrix3& Translate(const Vector3& vector);
	Matrix3& Scale(float x, float y, float z);
	Matrix3& Scale(const Vector3& vector);
	Matrix3& RotateAxis(const Vector3& axis, float theta);
	Matrix3& RotatePitch(float theta);
	Matrix3& RotateYaw(float theta);
	Matrix3& RotateRoll(float theta);
	Matrix3& RotateEulerAngles(float pitch, float yaw, float roll);
	Matrix3& RotateEulerAnglesYaw(float yaw, float pitch, float roll);
	Matrix3& Inverse();
	Matrix3& Invert(const Matrix3& other);
	Matrix3& Projection(float distance, float near, float far, float width, float height);
	Matrix3& ExtractRotation(const Matrix3& matrix, const Vector3& newTranslation = Vector3());
	Matrix3& TransformAround(const Vector3& point);
	Matrix3& Face(const Vector3& position, const Vector3& direction, const Vector3& globalUp = Vector3(0, 1, 0));

	Matrix3 Transposed() const { return Matrix3(*this).Transpose(); }
	Matrix3 Inverted() const { return Matrix3(*this).Inverse(); }
	Matrix3 Rotation(const Vector3& newTranslation = Vector3()) const { return Matrix3().ExtractRotation(*this, newTranslation); }
	Matrix3 TransformedAround(const Vector3& point) const { return Matrix3(*this).TransformAround(point); }

	Vector3 RightVector() const;
	Vector3 UpVector() const;
	Vector3 FrontVector() const;
	Vector3 Translation() const;
	Vector3 ExtractScale() const;
	float Det() const;

	Matrix3  operator+(const Matrix3& other) const;
	Matrix3  operator-(const Matrix3& other) const;
	Matrix3  operator-() const;
	Matrix3  operator*(const Matrix3& other) const;
	Vector3  operator*(const Vector3& other) const;
	Matrix3  operator*(float scalar) const;

	float* operator[](int row);
	const float* operator[](int row) const;

	operator std::string() const;

	Matrix3& operator=(const Matrix3& other);
	Matrix3& operator+=(const Matrix3& other);
	Matrix3& operator-=(const Matrix3& other);
	Matrix3& operator*=(const Matrix3& other);
	Matrix3& operator*=(float scalar);

	bool operator==(const Matrix3& other) const;
	bool operator!=(const Matrix3& other) const;

	static Matrix3 NewScale(float x, float y, float z) { return Matrix3().Scale(x, y, z); }
	static Matrix3 NewScale(const Vector3& vector) { return Matrix3().Scale(vector); }
	static Matrix3 AxisRotation(const Vector3& axis, float theta) { return Matrix3().RotateAxis(axis, theta); }
	static Matrix3 PitchRotation(float theta) { return Matrix3().RotatePitch(theta); }
	static Matrix3 YawRotation(float theta) { return Matrix3().RotateYaw(theta); }
	static Matrix3 RollRotation(float theta) { return Matrix3().RotateRoll(theta); }
	static Matrix3 EulerAnglesRotation(float pitch, float yaw, float roll) { return Matrix3().RotateEulerAngles(pitch, yaw, roll); }
	static Matrix3 EulerAnglesYawRotation(float yaw, float pitch, float roll) { return Matrix3().RotateEulerAnglesYaw(yaw, pitch, roll); }
	static Matrix3 NewProjection(float distance, float nearPlane, float farPlane, float width, float height) { return Matrix3().Projection(distance, nearPlane, farPlane, width, height); }
	static Matrix3 Facing(const Vector3& position, const Vector3& direction, const Vector3& globalUp = Vector3(0, 1, 0)) { return Matrix3().Face(position, direction, globalUp); }

private:
	static bool Compare(float x, float y, float epsilon);

	float Det(int y1, int y2, int x1, int x2) const;
	float ComponentMultiplication(const Matrix3 &rhs, int x, int y) const;

	Base_Class;

	Reflected_Type(Matrix3);
};

namespace Engine
{
	//Class_Inherits(Matrix3, CoreType);

	//Define_Value_Type(Matrix3);
	namespace LuaTypes
	{
		class Lua_Matrix3
		{
		public:
			template <typename T>
			class Converter
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, T defaultValue = T()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue)
				{
				}
			
				lua_State** LuaState = nullptr;
				const char* FuncName = "";
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				T DefaultValue = T();
			
				operator T() const
				{
					static const ReflectionData* type = nullptr; 
				
					if (type == nullptr)
						type = ReflectionData::GetType("Matrix3"); 
					
					int index = ArgumentNumber + (IsStatic ? 1 : 2); 
					
					if (lua_isuserdata(*LuaState, index))
					{
						LuaData* object = (LuaData*)lua_topointer(*LuaState, index); 
						
						if (object->Data == nullptr)
						{
							Lua::SetErrorMessage("Attempt to pass in static object to type '" + std::string("Matrix3") + "'"); 
							Lua::Error(*LuaState); 
						}
						else if (object->Meta != type)
							Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "Matrix3", object->Meta->Name.c_str()); 
							
						return *reinterpret_cast<Matrix3*>(object->Data);//ObjectReference(object->Reference).GetObjectData<Matrix3>();
					}
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "Matrix3", Lua::GetType(*LuaState, index)); 
						
						throw "shut up, compiler"; 
				}
			};
		
			class ReturnValue
			{
			public:
				const int ReturnValues = 1;
			
				lua_State** LuaState = nullptr;
			
			
				void operator()(const Matrix3& value)
				{
					static const ReflectionData* type = nullptr;
			
					if (type == nullptr)
						type = ReflectionData::GetType("Matrix3");
			
					try
					{
						//Handle<CoreClass> handle;
						//
						//type->Create(handle);


						//Handle<CoreClass> handle;
						//
						//object->Meta->Create(handle);

						void* data = type->CreateRaw();

						MakeLuaTypeReference(*LuaState, type, data);
				
						//Engine::MakeLuaTypeReference(*LuaState, handle.GetID(), type);
				
						new (data) Matrix3(value);
					}
					catch (std::string& err)
					{
						Lua::SetErrorMessage(err);
						Lua::Error(*LuaState);
					}
				}
			};
		};
	}

	template<>
	class CoreTypes<Matrix3>
	{
	public:
		typedef LuaTypes::Lua_Matrix3 LuaType;
	
		static const std::string GetTypeName() { return "Matrix3"; };
	};
}

Matrix3 operator*(float scalar, const Matrix3& rhs);
std::ostream& operator<<(std::ostream& out, const Matrix3& matrix);
