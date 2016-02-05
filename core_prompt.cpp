#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <shellapi.h>
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

#include "utility.h"

extern bool Running;

/*---------------------------------------
---------------------------------------*/
namespace Prompt
{
	/*---------------------------------------
	---------------------------------------*/
	static HANDLE ThreadHandle;
	static HANDLE ThreadMutex;

	/*---------------------------------------

	---------------------------------------*/
	void ShowOpenFilePrompt()
	{
		char buffer[64];

		// show the open file dialog
		printf("		> ");

		// read the file to open
		scanf("%s", buffer);

		// load it
		Lua::LoadFile(buffer);
	}

	/*---------------------------------------

	---------------------------------------*/
	DWORD WINAPI InputThread(LPVOID data)
	{
		char buffer[64];

		// get input
		while (Running)
		{
			// wait for the mutex to finish
			WaitForSingleObject(ThreadMutex, INFINITE);

			// show the command prompt
			printf("GerbilBot > ");

			// read the command
			scanf("%s", buffer);

			// openscript
			if (FStrEq(buffer, "openscript"))
			{
				// show the file open prompt
				ShowOpenFilePrompt();
			}
			else if (FStrEq(buffer, "quit"))
			{
				Running = false;
			}
			else
			{
				// fire event
				Lua::GetGlobal("__ConsoleCommand");
				Lua::Push(buffer);
				Lua::ProtectedCall(1, 0);
			}

			// release mutex
			ReleaseMutex(ThreadMutex);
		}

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	void Startup()
	{
		DWORD threadid;

		// setup the mutex
		ThreadMutex = CreateMutex(NULL, FALSE, NULL);

		// create
		ThreadHandle = CreateThread(
			NULL,
			0,
			InputThread,
			NULL,
			0,
			&threadid
			);

		// wait for the input thread to get done
		WaitForSingleObject(ThreadMutex, INFINITE);
	}

	/*---------------------------------------

	---------------------------------------*/
	void Shutdown()
	{
		// close them
		CloseHandle(ThreadHandle);
		CloseHandle(ThreadMutex);
	}

	/*---------------------------------------

	---------------------------------------*/
	void ReleaseMutex()
	{
		// release it
		::ReleaseMutex(ThreadMutex);
	}
}
