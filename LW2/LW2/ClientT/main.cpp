#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include "Winsock2.h"
#include "../ServerT/SocketError.h"
#include <iostream>
#include <cstring>
#include <string>
#define PORT 2000
using namespace SocketError;

int main()
{
    SOCKET clientSocket;
    WSADATA wsaData;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }
        if ((clientSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        if ((connect(clientSocket, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR) {
            throw SetErrorMsgText("connect:", WSAGetLastError());
        }
        std::cout << "Connection successful" << std::endl;
        int messageNum = 0;
        while (messageNum < 1000) {
            char outbuf[50] = "Hello from client ";
            strcat_s(outbuf, std::to_string(messageNum).c_str());
            int loutbuf = 0;
            if ((loutbuf = send(clientSocket, outbuf, strlen(outbuf) + 1, NULL)) == SOCKET_ERROR) {
                throw SetErrorMsgText("send:", WSAGetLastError());
            }

            std::cout << "Message" << messageNum << " sent to server" << std::endl;
            if ((loutbuf = recv(clientSocket, outbuf, sizeof(outbuf), NULL)) == SOCKET_ERROR) {
                throw SetErrorMsgText("recv:", WSAGetLastError());
            }
            std::cout << "server: " << outbuf << std::endl;
            messageNum++;
        }
        char outbuf[1]="";
        int loutbuf = 0;
        if ((loutbuf = send(clientSocket, outbuf, strlen(outbuf) + 1, NULL)) == SOCKET_ERROR) {
            throw SetErrorMsgText("send:", WSAGetLastError());
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
    getchar();
    return 0;
}