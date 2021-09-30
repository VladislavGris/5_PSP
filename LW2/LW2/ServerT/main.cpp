#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include "Winsock2.h"
#include "SocketError.h"
#define PORT 2000
#define MAX_CONN_COUNT 10
using namespace SocketError;

int main()
{
    SOCKET serverSocket;
    WSADATA wsaData;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText("bind:", WSAGetLastError());
        }
        if (listen(serverSocket, MAX_CONN_COUNT) == SOCKET_ERROR) {
            throw SetErrorMsgText("listen:", WSAGetLastError());
        }
        std::cout << "Listening port " << PORT << std::endl;
        SOCKET clientSocket;
        SOCKADDR_IN client;
        memset(&client, 0, sizeof(client));
        int lclient = sizeof(client);
        while (true) {
            std::cout << "Waiting for clients to connect..." << std::endl;
            if ((clientSocket = accept(serverSocket, (sockaddr*)&client, &lclient)) == INVALID_SOCKET) {
                throw SetErrorMsgText("accept:", WSAGetLastError());
            }

            std::cout << "IPAddr:" << inet_ntoa(client.sin_addr) << std::endl;
            std::cout << "Port:" << ntohs(client.sin_port) << std::endl;
            int messageCount = 0;
            clock_t start = clock();
            while (true) {
                char inbuf[50];
                int linbuf = 0;
                if ((linbuf = recv(clientSocket, inbuf, sizeof(inbuf), NULL)) == SOCKET_ERROR) {
                    throw SetErrorMsgText("recv:", WSAGetLastError());
                }
                if (strcmp(inbuf, "") == 0) {
                    break;
                }
                std::cout << inbuf << std::endl;
                if ((linbuf = send(clientSocket, inbuf, strlen(inbuf) + 1, NULL)) == SOCKET_ERROR) {
                    throw SetErrorMsgText("send:", WSAGetLastError());
                }
            }
            clock_t end = clock();
            std::cout << "Time: " << ((double)(end - start) / CLK_TCK) << std::endl;
        }
        
        if (closesocket(clientSocket) == SOCKET_ERROR) {
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        }
        if (closesocket(serverSocket) == SOCKET_ERROR) {
            throw SetErrorMsgText("closesocket:",WSAGetLastError());
        }
        if (WSACleanup() == SOCKET_ERROR) {
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
    }
    catch (std::string errorMsgText) {
        std::cout << std::endl << errorMsgText;
    }
    
    return 0;
}