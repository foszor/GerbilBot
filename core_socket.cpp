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

/*---------------------------------------
---------------------------------------*/
namespace Socket
{
	/*---------------------------------------
	---------------------------------------*/
	static SOCKET NetworkSocket = NULL;

	/*---------------------------------------

	---------------------------------------*/
	int LuaConnect(lua_State* L)
	{
		// make sure we're connected
		if (Invalid())
		{
			return 0;
		}

		// host and port
		const char* hostname = luaL_checkstring(L, 1);
		int port = luaL_checkint(L, 2);

		// connect
		Connect(hostname, port);

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	int LuaDisconnect(lua_State* L)
	{
		// make sure we're connected
		if (Invalid())
		{
			return 0;
		}

		// disconnect
		Disconnect();

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	int LuaSend(lua_State* L)
	{
		// make sure we're connected
		if (Invalid())
		{
			return 0;
		}

		// data
		const char* data = luaL_checkstring(L, 1);

		// send
		Send(data);

		return 0;
	}

	/*---------------------------------------

	---------------------------------------*/
	void Startup()
	{
		WSAData data;

		// startup
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			printf("Winsock startup error.");

			// shutdown
			Shutdown();
		}

		// register the lua functions
		Lua::Register(LuaConnect, "__Connect");
		Lua::Register(LuaDisconnect, "__Disconnect");
		Lua::Register(LuaSend, "__Send");
	}

	/*---------------------------------------

	---------------------------------------*/
	void Shutdown()
	{
		// cleanup
		WSACleanup();
	}

	/*---------------------------------------

	---------------------------------------*/
	void Connect(const char* hostname, unsigned short port)
	{
		// our address
		SOCKADDR_IN address;

		// get the ip of the host
		struct hostent* host = gethostbyname(hostname);
		address.sin_addr.S_un.S_addr = *((unsigned long*)host->h_addr);

		// set port
		address.sin_port = htons(port);

		// family
		address.sin_family = AF_INET;

		// create the socket
		NetworkSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Invalid())
		{
			return;
		}

		// connect
		connect(NetworkSocket, (LPSOCKADDR)&address, sizeof(struct sockaddr));
	}

	/*---------------------------------------

	---------------------------------------*/
	void Disconnect()
	{
		// close the socket if it's valid
		if (!Invalid())
		{
			closesocket(NetworkSocket);
		}
	}

	/*---------------------------------------

	---------------------------------------*/
	inline bool Invalid()
	{
		return NetworkSocket == INVALID_SOCKET;
	}

	/*---------------------------------------

	---------------------------------------*/
	int Receive(char* buffer, int size)
	{
		// clear the buffer
		memset(buffer, 0, size);

		// receive
		return recv(NetworkSocket, buffer, size, 0);
	}

	/*---------------------------------------

	---------------------------------------*/
	int Send(const char* buffer)
	{
		return send(NetworkSocket, buffer, (int)strlen(buffer), 0);
	}
}
