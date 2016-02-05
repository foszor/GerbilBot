#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

extern "C"
{
#include ".\lua-5.1.2\src\lua.h"
#include ".\lua-5.1.2\src\lualib.h"
#include ".\lua-5.1.2\src\lauxlib.h"
}

#include "core_lua.h"

/*---------------------------------------
---------------------------------------*/
namespace Lua
{
	/*---------------------------------------
	---------------------------------------*/
	static lua_State* LuaState = NULL;

	/*---------------------------------------

	---------------------------------------*/
	int LuaError(lua_State* L)
	{
		// write out the error
		printf("Error: %s\n", lua_tostring(L, -1));

		// call debug.traceback
		lua_pushliteral(L, "debug");
		lua_gettable(L, LUA_GLOBALSINDEX);
		//lua_getglobal(L, "_G");
		lua_pushliteral(L, "traceback");
		lua_gettable(L, -2);
		lua_pcall(L, 0, 0, 0);

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	int LuaInclude(lua_State* L)
	{
		// get the file to include
		const char* filename = luaL_checkstring(L, 1);

		// build new filename
		char buffer[256];
		sprintf(buffer, "lua/%s", filename);

		// load the file
		LoadFile(buffer);

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	void LoadDefaultLibraries()
	{
		// open io
		lua_pushcfunction(LuaState, luaopen_base);
		lua_call(LuaState, 0, 0);

		// open io
		lua_pushcfunction(LuaState, luaopen_table);
		lua_call(LuaState, 0, 0);

		// open io
		lua_pushcfunction(LuaState, luaopen_math);
		lua_call(LuaState, 0, 0);

		// open io
		lua_pushcfunction(LuaState, luaopen_string);
		lua_call(LuaState, 0, 0);

		// open io
		lua_pushcfunction(LuaState, luaopen_io);
		lua_call(LuaState, 0, 0);

		// open package
		lua_pushcfunction(LuaState, luaopen_package);
		lua_call(LuaState, 0, 0);

		// open debug
		lua_pushcfunction(LuaState, luaopen_debug);
		lua_call(LuaState, 0, 0);

		// register the include function
		Register(LuaInclude, "include");
		Register(LuaError, "err_func");
	}

	/*---------------------------------------

	---------------------------------------*/
	void InitializeLuaVM()
	{
		// setup the lua state
		LuaState = luaL_newstate();

		// load libraries
		LoadDefaultLibraries();
	}

	/*---------------------------------------

	---------------------------------------*/
	void CloseLuaVM()
	{
		// close it
		lua_close(LuaState);
	}

	/*---------------------------------------

	---------------------------------------*/
	void LoadFile(const char* filename)
	{
		printf("Loading lua file: %s\n", filename);

		// get err_func
		lua_pushliteral(LuaState, "err_func");
		lua_rawget(LuaState, LUA_GLOBALSINDEX);

		// load the file
		luaL_loadfile(LuaState, filename);

		// pcall
		lua_pcall(LuaState, 0, LUA_MULTRET, -2);
	}

	/*---------------------------------------

	---------------------------------------*/
	void PushErrorFunction()
	{
		// get err_func
		lua_pushliteral(LuaState, "err_func");
		lua_rawget(LuaState, LUA_GLOBALSINDEX);
	}

	/*---------------------------------------

	---------------------------------------*/
	void GetGlobal(const char* global)
	{
		lua_getglobal(LuaState, global);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Push(const char* str)
	{
		lua_pushstring(LuaState, str);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Push(float value)
	{
		lua_pushnumber(LuaState, value);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Push(int value)
	{
		lua_pushinteger(LuaState, value);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Push(bool value)
	{
		lua_pushboolean(LuaState, value);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Push()
	{
		lua_pushnil(LuaState);
	}

	/*---------------------------------------

	---------------------------------------*/
	void ProtectedCall(int args, int ret)
	{
		int base = lua_gettop(LuaState) - args;

		// get err_func
		lua_pushliteral(LuaState, "err_func");
		lua_rawget(LuaState, LUA_GLOBALSINDEX);
		lua_insert(LuaState, base);

		lua_pcall(LuaState, args, ret, base);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Pop(int amt)
	{
		lua_pop(LuaState, amt);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Register(lua_CFunction fn, const char* name)
	{
		printf("Registering: %s\n", name);
		lua_register(LuaState, name, fn);
	}
}
