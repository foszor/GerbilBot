/*---------------------------------------
---------------------------------------*/
namespace Socket
{
	/*---------------------------------------
	---------------------------------------*/
	void Startup();
	void Shutdown();

	/*---------------------------------------
	---------------------------------------*/
	void Connect(const char* hostname, unsigned short port);
	void Disconnect();

	/*---------------------------------------
	---------------------------------------*/
	bool Invalid();

	/*---------------------------------------
	---------------------------------------*/
	int Receive(char* buffer, int size);
	int Send(const char* buffer);
}
