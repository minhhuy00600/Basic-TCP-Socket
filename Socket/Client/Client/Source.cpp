#include"Header.h"

/*
void nhan_danhba(char Buffer[4096], SOCKET& socket_descriptor)
{
	int check;
	while (true)
	{
		Sleep(20);
		check = recv(socket_descriptor, Buffer, sizeof Buffer, 0);
		if (check != SOCKET_ERROR && strcmp(Buffer, "end") != 0)
		{
			send(socket_descriptor, "ok", sizeof "ok", 0);
			std::cout << Buffer << endl;
		}
		else break;
	}
}
*/

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
		cout << "Socket creation failed : " << WSAGetLastError() << endl;
		cout << "Socket Descriptor : " << socket_descriptor;
	}

	struct sockaddr_in {
		short sin_family;
		u_short sin_port;
		struct in_addr sin_addr;
		char sin_zero[8];
	};

	struct hostent {
		char FAR* h_name;
		char FAR* FAR* h_aliases;
		short h_addrtype;
		short h_length;
		char FAR* FAR* h_addr_list;
	};

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(50000);

	strcpy_s(ServerName, "localhost");

	printf("Gethostbyname(\"%s\")\n", ServerName);
	if (pHostEnt = gethostbyname(ServerName))
	{
		memcpy(&sin.sin_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
		printf("Address Length : %d\n", pHostEnt->h_length);
		printf("Host Address : %s\n", inet_ntoa(sin.sin_addr));
		printf("Host Name : %s\n", pHostEnt->h_name);
		printf("\n");
	}
	else
	{
		printf("Can't get %s\" host entry : %d\n", ServerName, WSAGetLastError());
		WSACleanup();
		return 0;
	}
	cout << "connect()\n";

	retcode = connect(socket_descriptor, (struct sockaddr*)&sin, sizeof(sin));
	if (retcode == SOCKET_ERROR)
	{
		cout << "Connect failed : " << WSAGetLastError();
		return 0;
	}
	printf("Return Code : %d\n", retcode);
	cout << endl;

	cout << "send()\n";  //Gui tin nhan len server .
	retcode = send(socket_descriptor, Message, sizeof Message, 0);
	if (retcode == SOCKET_ERROR)
	{
		cout << "Send failed : " << WSAGetLastError();
		return 0;
	}
	printf("Bytes Sent : %d\n", retcode);
	cout << endl;

	cout << "recv()\n";

	length = recv(socket_descriptor, Buffer, sizeof Buffer, 0);
	if (length == SOCKET_ERROR)
	{
		cout << "Receive failed : " << WSAGetLastError();
		return 0;
	}
	printf("Bytes received : %d\n", length);
	printf("Message : %s\n", Buffer);
	cout << endl << endl;
	std::system("pause");



	std::system("CLS");

	char num[25];
	std::cout << "1. Nhan danh ba." << endl;
	std::cout << "2. Lay thong tin 1 thanh vien." << endl;

	std::cout << "Input number you want : ";
	cin >> num;
	send(socket_descriptor, num, sizeof num, 0);
	recv(socket_descriptor, Buffer, sizeof Buffer, 0);
	
	//Cau 1:
	if (strcmp(num, "1") == 0)
	{
		std::system("CLS");
		std::cout << Buffer << endl;
		int check;
		while (true)
		{
			check = recv(socket_descriptor, Buffer, sizeof Buffer, 0);
			if (check != SOCKET_ERROR && strcmp(Buffer, "end") != 0)
			{
				send(socket_descriptor, "ok", sizeof "ok", 0);
				std::cout << Buffer << endl;
			}
			else break;
		}
	}

	//Cau 2:
	else if (strcmp(num, "2") == 0)
	{
		std::system("CLS");
		std::cout << Buffer << endl;
		char input[255];
		cin >> input;
		std::system("CLS");
		send(socket_descriptor, input, sizeof input, 0);
		int check;
		
		while (true)
		{
			check = recv(socket_descriptor, Buffer, sizeof Buffer, 0);
			if (check != SOCKET_ERROR && strcmp(Buffer, "end") != 0)
			{
				send(socket_descriptor, "ok", sizeof "ok", 0);
				std::cout << Buffer << endl;
			}
			else break;
		}
	}
	std::system("pause");


	cout << "closesocket()\n";

	retcode = closesocket(socket_descriptor);
	if (retcode == SOCKET_ERROR)
	{
		cout << "Close socket failed : " << WSAGetLastError();
		return 0;
	}
	printf("Return Code : %d\n", retcode);
	cout << endl;

	WSACleanup();
	return 0;
}