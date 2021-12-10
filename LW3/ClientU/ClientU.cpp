#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include "Winsock2.h"
#include "../SocketError.h"
#include <iostream>
#include <cstring>
#include <string>
#define PORT 2000
#define ADDR "127.0.0.1"
#define ITERATION_COUNT 10000
using namespace SocketError;

int main()
{
    SOCKET clientSocket;
    WSADATA wsaData;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }
        if ((clientSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = inet_addr(ADDR);
        int ls = sizeof(serv);

        char ibuf[50];
        int lobuf = 0;
        int i = 1;
        clock_t start, end;
        start = clock();
        while (i <= ITERATION_COUNT) {
            char obuf[50] = "Hello";
            //strcat_s(obuf, std::to_string(i++).c_str());
            if (lobuf = sendto(clientSocket, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
                throw SetErrorMsgText("send: ", WSAGetLastError());
            }
            //Sleep(1000);
            /*if (lobuf = recvfrom(clientSocket, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &ls) == SOCKET_ERROR) {
                throw SetErrorMsgText("recv: ", WSAGetLastError());
            }
            std::cout << ibuf << std::endl;*/
        }
        end = clock();
        std::cout << "Elapsed time: " << ((double)(end - start) / CLK_TCK) << std::endl;
        char obuf[2] = "";
        if (lobuf = sendto(clientSocket, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText("send: ", WSAGetLastError());
        }

        if (closesocket(clientSocket) == SOCKET_ERROR) {
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