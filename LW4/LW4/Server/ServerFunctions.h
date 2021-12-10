#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include "Winsock2.h"
#include <string>
#include "../SocketError.h"

namespace Server {

	bool GetRequestFromClient(SOCKET* ss, char* name, short port, struct sockaddr* from, int* flen);
	bool PutAnswerToClient(SOCKET* ss, char* name, struct sockaddr* to, int* lto);
	bool GetServer(char* call, short port, struct sockaddr* from, int* flen);
	bool GetServerByName(const char* name, const char* call, struct sockaddr* from, short port, int* flen);
}