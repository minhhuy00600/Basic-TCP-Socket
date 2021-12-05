#include"Header_client.h"

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int retcode;

	SOCKET socket_descriptor = INVALID_SOCKET; // Lang nghe ket noi tu client
	
	char ServerName[64];
	HOSTENT* pHostEnt;
	struct sockaddr_in sin;

	char Message[] = "Greetings from the client";

	char Buffer[4096];
	int length;

	cout << "WSAStartup()" << endl;
	wVersionRequested = MAKEWORD(2, 2);
	retcode = WSAStartup(wVersionRequested, &wsaData);
	if (retcode != 0)
	{
		cout << "Startup failed : " << WSAGetLastError();
		closesocket(socket_descriptor);
		WSACleanup();
		return 0;
	}
	cout << "Return Code : " << retcode << endl;
	printf("Version Used : %i.%i\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
	printf("Version Supported : %i.%i\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
	printf("Implementation : %s\n", wsaData.szDescription);
	printf("System Status : %s\n", wsaData.szSystemStatus);
	cout << endl;

	if (LOBYTE(wsaData.wVersion) != LOBYTE(wVersionRequested) || HIBYTE(wsaData.wVersion) != HIBYTE(wVersionRequested))
	{
		printf("Supported version is too low\n");
		WSACleanup();
		return 0;
	}

	cout << "Socket()\n";
	socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == INVALID_SOCKET)
	{

	}
	
}


