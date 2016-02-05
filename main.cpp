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
#include "core_socket.h"
#include "core_prompt.h"

/*---------------------------------------
---------------------------------------*/
#pragma comment( lib, "wsock32.lib" )
#pragma comment( lib, "ws2_32.lib" )

/*---------------------------------------
---------------------------------------*/
bool Running = true;

/*---------------------------------------

---------------------------------------*/
int main(int argc, char** argv)
{
	// setup the input thread.
	Prompt::Startup();

	// initialize lua
	Lua::InitializeLuaVM();

	// initialize winsock
	Socket::Startup();

	// load initialization file
	Lua::LoadFile("lua/wheel.lua");

	// setupt he receive buffer
	char buffer[512];
	int bytesread = 0;

	// Network loop
	while (Running)
	{
		// handle network code.
		if (!Socket::Invalid())
		{
			// receive
			bytesread = Socket::Receive(buffer, 512);
			if (bytesread > 0)
			{
				// output to lua
				Lua::GetGlobal("__RawData");
				Lua::Push(buffer);
				Lua::ProtectedCall(1, 0);
			}
			else if (bytesread == 0)
			{
				// fire event
				Lua::GetGlobal("__LostConnection");
				Lua::ProtectedCall(0, 0);

				// close it off
				Socket::Disconnect();
			}
		}

		// release mutex
		Prompt::ReleaseMutex();
	}

	// disconnect
	Socket::Disconnect();

	// shutdown winsock
	Socket::Shutdown();

	// shutdown lua
	Lua::CloseLuaVM();

	// destroy the input thread
	Prompt::Shutdown();

	return 0;
}
