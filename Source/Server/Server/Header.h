#include<iostream>
#include<winSock2.h>
#include<stdio.h>
#include<tchar.h>
#include<WS2tcpip.h>
#include<string>
#include<Windows.h>
#include<fstream>
#include <cstdlib>


#define DEFAULT_PORT "27015"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

const int MAX_CLIENTS = 5;