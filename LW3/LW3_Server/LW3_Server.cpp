#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include "Winsock2.h"
#include "../SocketError.h"
#define PORT 2000
using namespace SocketError;

int main()
{
    SOCKET serverSocket;
    WSADATA wsaData;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText("bind:", WSAGetLastError());
        }

        SOCKADDR_IN client;
        memset(&client, 0, sizeof(client));
        int lclient = sizeof(client);
        char inbuf[50];
        int lb = 0;
        std::cout << "Waiting for data..." << std::endl;
        int i = 0;
        while (true) {
            if (lb = recvfrom(serverSocket, inbuf, sizeof(inbuf), NULL, (sockaddr*)&client, &lclient) == SOCKET_ERROR) {
                throw SetErrorMsgText("recv: ", WSAGetLastError());
            }
            i += 1;
            std::cout << i << " " << inbuf << std::endl;
            if (strcmp(inbuf,"") == 0) {
                break;
            }
            /*if (lb = sendto(serverSocket, inbuf, strlen(inbuf) + 1, NULL, (sockaddr*)&client, lclient) == SOCKET_ERROR) {
                throw SetErrorMsgText("send: ", WSAGetLastError());
            }
            Sleep(10);*/
        }
        std::cout << i << std::endl;
        if (closesocket(serverSocket) == SOCKET_ERROR) {
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        }
        if (WSACleanup() == SOCKET_ERROR) {
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
    }
    catch (std::string errorMsgText) {
        std::cout << std::endl << errorMsgText;
    }
    system("pause");
    return 0;
}