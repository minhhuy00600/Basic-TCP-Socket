#include "Header.h"

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int retcode;

	SOCKET socket_descriptor = INVALID_SOCKET; // Lang nghe ket noi tu client
	struct sockaddr_in sin;

	SOCKET new_socket; //Trao doi du lieu voi client
	struct sockaddr_in new_sin;
	
	char ClientAddr[4]; //So luong port
	HOSTENT* pClientHostEnt;

	char Buffer[4096];
	int length;
	char Message[] = "Message from the server.";

	cout << "Initializing Winsock... " << endl;

	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsOk != 0)
	{
		cout << "Can't initialize winsock. Exiting... !" << WSAGetLastError()<< endl;
		WSACleanup();
		return 0;
	}

	cout << "Server started..." << endl<<endl;
	/*
	ZeroMemory(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(50000);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(c);

	while (true)
	{
		SOCKET NewSocket_des = INVALID_SOCKET;
		NewSocket_des = accept(socket_descriptor, NULL, NULL);

		if (NewSocket_des == INVALID_SOCKET)
		{

		}

	}
	*/

	socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

	ZeroMemory(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(50000);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(c


	cout << "Bind()..." << endl;
	retcode = bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));
	if (retcode == SOCKET_ERROR)
	{
		cout << "Bind failed with error : " << WSAGetLastError()<<endl;
		closesocket(socket_descriptor);
		WSACleanup();
		return 1;
	}
	cout << "Return code: " << retcode << endl << endl;

	cout << "Listening ..." << endl;
	retcode = listen(socket_descriptor, 0);
	if (retcode == SOCKET_ERROR)
	{
		cout << "Listening failed ! Exiting ... " << WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}
	cout << "Return code: " << retcode << endl<<endl;
	
	cout << "Accept()" << endl;
	int addrlen = sizeof(new_sin);
	new_socket = accept(socket_descriptor, (struct sockaddr*)&new_sin, &addrlen);
	if (new_socket == INVALID_SOCKET)
	{
		cout << "Accept failed ..." << WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}

	cout << "New Socket Descriptor : " << new_socket << endl;
	cout << "Remote Port : " << ntohs(new_sin.sin_port) << endl;
	cout << "Host Address : " << inet_ntoa(new_sin.sin_addr) << endl;
	cout << endl;

	cout << "Gethostbyaddr : " << inet_ntoa(new_sin.sin_addr) << endl;
	memcpy(ClientAddr, &new_sin.sin_addr, sizeof(ClientAddr));
	pClientHostEnt = gethostbyaddr(ClientAddr, sizeof(ClientAddr), PF_INET);
	if (pClientHostEnt == NULL)
	{
		cout << "Get host by address failed :" << WSAGetLastError()<< endl;
		WSACleanup();
		return 1;
	}
	cout << "Remote Host : " << pClientHostEnt->h_name << endl;
	cout << endl;

	cout << "recv()" << endl;

	length = recv(new_socket, Buffer, sizeof Buffer, 0);
	if (length == SOCKET_ERROR)
	{
		cout << "Receive failed." << WSAGetLastError()<< endl;
		WSACleanup();
		return 0;
	}
	cout << "Bytes received : " << length << endl;
	cout << "Message :" << Buffer << endl;

	cout << "send()" << endl;
	retcode = send(new_socket, Message, sizeof Message, 0);
	if (retcode == SOCKET_ERROR)
	{
		cout << "Send failed ." << WSAGetLastError()<< endl;
		WSACleanup();
		return 1;
	}
	cout << "Bytes Sent : " << retcode << endl;

	cout << "closesocket()" << endl;
	retcode = closesocket(new_socket); //dong ket noi trao doi du lieu voi client
	if (retcode == SOCKET_ERROR)
	{
		cout << "Close socket failed ." << WSAGetLastError()<< endl;
		WSACleanup();
		return 1;
	}
	cout << "Return Code : " << retcode << endl;

	retcode = closesocket(socket_descriptor); //dong socket lang nghe ket noi
	if (retcode == SOCKET_ERROR)
	{
		cout << "Close socket failed : " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Return Code : " << retcode << endl;
	WSACleanup();
	cout << "Server has ended !" << endl;
	system("pause");
	return 0;

}