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


void DownloadFile(SOCKET socket_descriptor) {
	if (socket_descriptor == NULL) {
		return;
	}
	while (1) {
		printf("Input local filename: ");
		char localfile[1024];
		cin >> localfile;
		if (localfile[0] == '.') {
			send(socket_descriptor, localfile, sizeof(localfile), 0);
			break;
		}
		printf("Input remote filename: ");
		char filename[1024];
		gets_s(filename, 1024);
		if (filename[0] == '.') {
			send(socket_descriptor, filename, sizeof(filename), 0);
			break;
		}
		send(socket_descriptor, filename, sizeof(filename), 0);
		char GotFileSize[1024];
		recv(socket_descriptor, GotFileSize, 1024, 0);
		long FileSize = atoi(GotFileSize);
		long SizeCheck = 0;
		FILE* fp = fopen(localfile, "w");
		char* mfcc;
		if (FileSize > 1499) {
			mfcc = (char*)malloc(1500);
			while (SizeCheck < FileSize) {
				int Received = recv(socket_descriptor, mfcc, 1500, 0);
				SizeCheck += Received;
				fwrite(mfcc, 1, Received, fp);
				fflush(fp);
				printf("Filesize: %d\nSizecheck: %d\nReceived: %d\n\n", FileSize, SizeCheck, Received);
			}
		}
		else {
			mfcc = (char*)malloc(FileSize + 1);
			int Received = recv(socket_descriptor, mfcc, FileSize, 0);
			fwrite(mfcc, 1, Received, fp);
			fflush(fp);
		}
		fclose(fp);
		Sleep(500);
		free(mfcc);
	}
}

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

	std::cout << "WSAStartup()" << endl;
	wVersionRequested = MAKEWORD(2, 2);
	retcode = WSAStartup(wVersionRequested, &wsaData);
	if (retcode != 0)
	{
		std::cout << "Startup failed : " << WSAGetLastError();
		closesocket(socket_descriptor);
		WSACleanup();
		return 0;
	}
	std::cout << "Return Code : " << retcode << endl;
	printf("Version Used : %i.%i\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
	printf("Version Supported : %i.%i\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
	printf("Implementation : %s\n", wsaData.szDescription);
	printf("System Status : %s\n", wsaData.szSystemStatus);
	std::cout << endl;

	if (LOBYTE(wsaData.wVersion) != LOBYTE(wVersionRequested) || HIBYTE(wsaData.wVersion) != HIBYTE(wVersionRequested))
	{
		printf("Supported version is too low\n");
		WSACleanup();
		return 0;
	}

	std::cout << "Socket()\n";
	socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed : " << WSAGetLastError() << endl;
		std::cout << "Socket Descriptor : " << socket_descriptor;
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
	std::cout << "connect()\n";

	retcode = connect(socket_descriptor, (struct sockaddr*)&sin, sizeof(sin));
	if (retcode == SOCKET_ERROR)
	{
		std::cout << "Connect failed : " << WSAGetLastError();
		return 0;
	}
	printf("Return Code : %d\n", retcode);
	std::cout << endl;

	std::cout << "send()\n";  //Gui tin nhan len server .
	retcode = send(socket_descriptor, Message, sizeof Message, 0);
	if (retcode == SOCKET_ERROR)
	{
		std::cout << "Send failed : " << WSAGetLastError();
		return 0;
	}
	printf("Bytes Sent : %d\n", retcode);
	std::cout << endl;

	std::cout << "recv()\n";

	length = recv(socket_descriptor, Buffer, sizeof Buffer, 0);
	if (length == SOCKET_ERROR)
	{
		std::cout << "Receive failed : " << WSAGetLastError();
		return 0;
	}
	printf("Bytes received : %d\n", length);
	printf("Message : %s\n", Buffer);
	std::cout << endl << endl;
	std::system("pause");



	std::system("CLS");

	char num[25];
	std::cout << "1. Nhan danh ba." << endl;
	std::cout << "2. Lay thong tin 1 thanh vien." << endl;
	std::cout << "3.Download file anh." << endl;

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

	//Cau 3:
	else if (strcmp(num, "3") == 0)
	{
		if (socket_descriptor == NULL)
		{
			return 0;
		}
		std::cout << Buffer << endl;


		printf("Input filename: ");

		char localfile[1024];
		char packet[1024]; // Nhan du lieu file tu server
		int check; // Check du lieu
		cin >> localfile;

		FILE* fp = fopen(localfile, "w");

		send(socket_descriptor, localfile, sizeof localfile, 0);
		while (true)
		{
			check = recv(socket_descriptor, packet, sizeof packet, 0);
			if (check != INVALID_SOCKET && strcmp(packet, "end") != 0)
			{
				send(socket_descriptor, "ok", sizeof "ok", 0);
				fwrite(packet, strlen(packet) + 1, 1, fp);
				break;
			}
			else break;
		}
		fclose(fp);
	}
			/*
			if (localfile[0] == '.') {
				send(socket_descriptor, localfile, sizeof(localfile), 0);
				break;
			}
			printf("Input remote filename: ");
			char filename[1024];
			gets_s(filename, 1024);
			if (filename[0] == '.') {
				send(socket_descriptor, filename, sizeof(filename), 0);
				break;
			}
			
			send(socket_descriptor, filename, sizeof(filename), 0);
			char GotFileSize[1024];
			recv(socket_descriptor, GotFileSize, 1024, 0);
			long FileSize = atoi(GotFileSize);
			long SizeCheck = 0;
			
			FILE* fp = fopen(localfile, "w");
			char* mfcc;
			if (FileSize > 1499) {
				mfcc = (char*)malloc(1500);
				while (SizeCheck < FileSize) {
					int Received = recv(socket_descriptor, mfcc, 1500, 0);
					SizeCheck += Received;
					fwrite(mfcc, 1, Received, fp);
					fflush(fp);
					printf("Filesize: %d\nSizecheck: %d\nReceived: %d\n\n", FileSize, SizeCheck, Received);
				}
			}
			else {
				mfcc = (char*)malloc(FileSize + 1);
				int Received = recv(socket_descriptor, mfcc, FileSize, 0);
				fwrite(mfcc, 1, Received, fp);
				fflush(fp);
			}
			fclose(fp);
			Sleep(500);
			free(mfcc);
			*/

		// DownloadFile(socket_descriptor);

		/*
		printf("Receiving file .. \n");

		int Size;
		char* Filesize = new char[1024];

		if (recv(socket_descriptor, Filesize, 1024, 0)) // File size
		{
			Size = atoi((const char*)Filesize);
			printf("File size: %d\n", Size);
		}

		char* temp = new char[Size];

		if (recv(socket_descriptor, temp, Size, 0)) // File Binary
		{
			FILE* File;
			File = fopen("19127424.png", "wb");
			fwrite((const char*)temp, 1, Size, File);
			fclose(File);
		}
		*/
	std::system("pause");


	std::cout << "closesocket()\n";

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