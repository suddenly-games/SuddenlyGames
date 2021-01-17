#pragma once

class ReflectionData;
struct lua_State;
struct luaL_Reg;

/*
Reflected function -> Lua metamethod overload mapping:

operator string -> __tostring
operator()		-> __call
GetSize			-> __len
LuaGCed			-> __gc
operator-		-> __unarym
operator+		-> __add
operator-		-> __sub
operator*		-> __mul
operator/		-> __div
FloorDivide		-> __idiv
operator%		-> __mod
Pow				-> __pow
Concat			-> __concat
operator&		-> __band
operator|		-> __bor
operator^		-> __bxor
operator~		-> __bnot
operator<<		-> __bshl
operator>>		-> __bshr
operator==		-> __eq
operator<		-> __lt
operator<=		-> __le
*/

namespace Engine
{
	class ObjectMetatable
	{
	public:
		static int __index(lua_State*);
		static int __newindex(lua_State*);
		static int __tostring(lua_State*);
		static int __call(lua_State*);
		static int __len(lua_State*);
		static int __gc(lua_State*);
		static int __unaryMinus(lua_State*);
		static int __add(lua_State*);
		static int __subtract(lua_State*);
		static int __multiply(lua_State*);
		static int __divide(lua_State*);
		static int __floorDivide(lua_State*);
		static int __modulus(lua_State*);
		static int __power(lua_State*);
		static int __concatenate(lua_State*);
		static int __bitwiseAnd(lua_State*);
		static int __bitwiseOr(lua_State*);
		static int __bitwiseXOr(lua_State*);
		static int __bitwiseNot(lua_State*);
		static int __bitwiseShiftLeft(lua_State*);
		static int __bitwiseShiftRight(lua_State*);
		static int __equals(lua_State*);
		static int __lessThan(lua_State*);
		static int __lessThanEqualTo(lua_State*);
		static const luaL_Reg Metatable[25];
	};

	struct ObjectConstructorMetatable
	{
		static int __tostring(lua_State*);
		static int __call(lua_State*);
		static const luaL_Reg Metatable[3];
	};

	struct TypeConstructorMetatable
	{
		static int __tostring(lua_State*);
		static int __call(lua_State*);
		static const luaL_Reg Metatable[6];
	};
}
