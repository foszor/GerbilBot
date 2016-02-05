/*---------------------------------------
---------------------------------------*/
namespace Lua
{
	/*---------------------------------------
	---------------------------------------*/
	void LoadDefaultLibraries();
	void InitializeLuaVM();
	void CloseLuaVM();

	/*---------------------------------------
	---------------------------------------*/
	void LoadFile(const char* filename);

	/*---------------------------------------
	---------------------------------------*/
	void GetGlobal(const char* global);
	void Push(const char* str);
	void Push(float value);
	void Push(int value);
	void Push(bool value);
	void Push();
	void Pop(int amt);
	void ProtectedCall(int args, int ret);
	void Register(lua_CFunction fn, const char* name);
	void PushErrorFunction();
}
