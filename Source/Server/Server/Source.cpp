#include "Header.h"

int64_t GetFileSize(const std::string& fileName) {
	// no idea how to get filesizes > 2.1 GB in a C++ kind-of way.
	// I will cheat and use Microsoft's C-style file API
	FILE* f;
	if (fopen_s(&f, fileName.c_str(), "rb") != 0) {
		return -1;
	}
	_fseeki64(f, 0, SEEK_END);
	const int64_t len = _ftelli64(f);
	fclose(f);
	return len;
}


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

void DownloadFile(SOCKET Socket)
{
	if (Socket == NULL) {
		return;
	}
	char filename[1024];
	char packet[1024];
	char check[20] = "ok";
	recv(Socket, filename, sizeof filename, 0);
	
	int size = GetFileSize(string(filename));
	int i = 0;
	/*
	int32_t conv = htonl(size);
	char* file_size = (char*)&conv;

	send(Socket, file_size, sizeof packet, 0);
	*/
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		send(Socket, "The file is not exist", sizeof "The file is not exist", 0);
		return;
	}
	else
	{
		fseek(fp, 0, SEEK_SET);
		while (i < size)
		{
			fread(packet, strlen(packet) + 1, 1, fp);
			if (strcmp(check, "ok") == 0) send(Socket, packet, sizeof packet, 0);
			recv(Socket, check, sizeof check, 0);
			i++;
		}
		send(Socket, "end", sizeof "end", 0);
		fclose(fp);
	}

	/*
	while (true) {
		char filename[1024];
		recv(Socket, filename, sizeof(filename), 0);
		if (filename[0] == '.') {
			break;
		}
		FILE* fp = fopen(filename, "r");
		fseek(fp, 0, SEEK_END);
		long FileSize = ftell(fp);
		char GotFileSize[1024];
		_itoa_s(FileSize, GotFileSize, 10);
		send(Socket, GotFileSize, 1024, 0);
		rewind(fp);
		long SizeCheck = 0;
		char* mfcc;
		if (FileSize > 1499) {
			mfcc = (char*)malloc(1500);
			while (SizeCheck < FileSize) {
				int Read = fread_s(mfcc, 1500, sizeof(char), 1500, fp);
				int Sent = send(Socket, mfcc, Read, 0);
				SizeCheck += Sent;
			}
		}
		else {
			mfcc = (char*)malloc(FileSize + 1);
			fread_s(mfcc, FileSize, sizeof(char), FileSize, fp);
			send(Socket, mfcc, FileSize, 0);
		}
		fclose(fp);
		Sleep(500);
		free(mfcc);
	}
	return;
	*/
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
	else if (strcmp(Buffer, "3") == 0)
	{
		send(new_socket, "Download file ...", sizeof "Download file ...", 0);
		DownloadFile(new_socket);
		/*
		FILE* File;
		char* Buffer;
		unsigned long Size;

		File = fopen("19127424.png", "rb");
		if (!File)
		{
			printf("Error while readaing the file\n");
		}

		fseek(File, 0, SEEK_END);
		Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		Buffer = new char[Size];

		fread(Buffer, Size, 1, File);
		char cSize[MAX_PATH];
		sprintf(cSize, "%i", Size);

		fclose(File);

		send(new_socket, cSize, MAX_PATH, 0); // File size
		send(new_socket, Buffer, Size, 0); // File Binary
		*/
	}

	cout<< "closesocket()" << endl << endl;
	free(Buffer);
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