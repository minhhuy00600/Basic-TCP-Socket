#include "Header.h"

void truyvan_danhba(ifstream& f, SOCKET &new_socket) //Yeu cau 1
{
	f.open("thanhvien.txt", ios::app);
	char temp[255];
	char check[255] = "ok";
	while (!f.eof())
	{
		f.getline(temp, 255);
		if(strcmp(check, "ok") == 0) send(new_socket, temp, sizeof temp, 0);
		recv(new_socket, check, sizeof check, 0);
		Sleep(20);
	}
	send(new_socket, "end", sizeof "end", 0);
	std::system("pause");
	f.close();
}

void truyvan_1_danhba(ifstream& f, SOCKET &new_socket, char yeucau[]) //Yeu cau 2
{
	f.open("thanhvien.txt", ios::app);
	char temp[255];
	char check[255] = "ok";
	while (!f.eof())
	{
		f.getline(temp, 255);
		if(strcmp(temp, yeucau) == 0)
		{
			int i = 0;
			while(true)
			{
				std::cout << temp << endl;
			if (strcmp(check, "ok") == 0) send(new_socket, temp, sizeof temp, 0);
				recv(new_socket, check, sizeof check, 0);
				f.getline(temp, 255);
			if (strcmp(temp, " ") == 0 || strlen(temp) == 0) break;
			i++;
			Sleep(20);
			}
		}
		Sleep(20);
	}
	//send(new_socket, "Thanh vien ban nhap khong hop le", sizeof "Thanh vien ban nhap khong hop le", 0);
	//recv(new_socket, check, sizeof check, 0);
	if (strcmp(check, "ok") == 0) send(new_socket, "end", sizeof "end", 0);
	std::system("pause");
	f.close();
}

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

	ifstream open; // doc file

	std::cout << "Initializing Winsock... " << endl;

	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsOk != 0)
	{
		std::cout << "Can't initialize winsock. Exiting... !" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	std::cout << "Server started..." << endl << endl;

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
		std::cout << "Socket creation failed" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	ZeroMemory(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(50000);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr(c


	std::cout << "Bind()..." << endl;
	retcode = bind(socket_descriptor, (struct sockaddr*)&sin, sizeof(sin));
	if (retcode == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error : " << WSAGetLastError() << endl;
		closesocket(socket_descriptor);
		WSACleanup();
		return 1;
	}
	std::cout << "Return code: " << retcode << endl << endl;

	std::cout << "Listening ..." << endl;
	retcode = listen(socket_descriptor, 0);
	if (retcode == SOCKET_ERROR)
	{
		std::cout << "Listening failed ! Exiting ... " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	std::cout << "Return code: " << retcode << endl << endl;

	std::cout << "Accept()" << endl;
	int addrlen = sizeof(new_sin);
	new_socket = accept(socket_descriptor, (struct sockaddr*)&new_sin, &addrlen);

	if (new_socket == INVALID_SOCKET)
	{
		std::cout << "Accept failed ..." << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	std::cout << "New Socket Descriptor : " << new_socket << endl;
	std::cout << "Remote Port : " << ntohs(new_sin.sin_port) << endl;
	std::cout << "Host Address : " << inet_ntoa(new_sin.sin_addr) << endl;
	std::cout << endl;

	std::cout << "Gethostbyaddr : " << inet_ntoa(new_sin.sin_addr) << endl;
	memcpy(ClientAddr, &new_sin.sin_addr, sizeof(ClientAddr));
	pClientHostEnt = gethostbyaddr(ClientAddr, sizeof(ClientAddr), PF_INET);
	if (pClientHostEnt == NULL)
	{
		std::cout << "Get host by address failed :" << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	std::cout << "Remote Host : " << pClientHostEnt->h_name << endl;
	std::cout << endl;

	std::cout << "recv()" << endl;  // Nhan tin nhan tu client

	length = recv(new_socket, Buffer, sizeof Buffer, 0);
	if (length == SOCKET_ERROR)
	{
		std::cout << "Receive failed." << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	std::cout << "Bytes received : " << length << endl;
	std::cout << "Message :" << Buffer << endl;


	std::cout << "send()" << endl;
	retcode = send(new_socket, Message, sizeof Message, 0);
	if (retcode == SOCKET_ERROR)
	{
		std::cout << "Send failed ." << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	std::cout << "Bytes Sent : " << retcode << endl << endl;
	std::system("pause");


	// Code danh ba o day
	cout << "Lang nghe client";
	recv(new_socket, Buffer, sizeof Buffer, 0);
	if (strcmp(Buffer, "1") == 0)
	{
		std::system("CLS");
		send(new_socket, "Truy van thong tin danh ba :", sizeof "Truy van thong tin danh ba :", 0);
		std::system("CLS");
		cout << "Truy van danh ba ..." << endl << endl;
		truyvan_danhba(open, new_socket);
		std::cout << endl;
	}
	else if (strcmp(Buffer, "2") == 0)
	{
		std::system("CLS");
		send(new_socket, "Truy cap thong tin 1 thanh vien.\n Input MSSV : ",
			sizeof "Truy cap thong tin 1 thanh vien.\n Input MSSV : ", 0);
		recv(new_socket, Buffer, sizeof Buffer, 0);
		truyvan_1_danhba(open, new_socket, Buffer);
	}

	cout<< "closesocket()" << endl << endl;

	retcode = closesocket(new_socket); //dong ket noi trao doi du lieu voi client
	if (retcode == SOCKET_ERROR)
	{
		cout << "Close socket failed ." << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Return Code : " << retcode << endl << endl;

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