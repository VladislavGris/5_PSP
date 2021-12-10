#include <iostream>
#include "ServerFunctions.h"
#define PORT 2000
#define NAME "Hello"

int main()
{
    SOCKET serverSocket;
    WSADATA wsaData;
    SOCKADDR_IN client;
    int lc = sizeof(client);
    char name[10] = "Hello";
    char fakeName[10] = "Hello1";
    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw SocketError::SetErrorMsgText("Startup:", WSAGetLastError());
        }

        Server::GetServer(name, PORT, (sockaddr*)&client, &lc);
        system("pause");
        

        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
            throw SocketError::SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SocketError::SetErrorMsgText("bind:", WSAGetLastError());
        }

        hostent* hostnameServer = gethostbyaddr((char*)&serv.sin_addr, sizeof(serv.sin_addr), AF_INET);
        std::cout << "Server hostname: " << hostnameServer->h_name << std::endl;

        while (true) {
            if (Server::GetRequestFromClient(&serverSocket, name, htons(PORT), (sockaddr*)&client, &lc)) {
                hostent* hostnameClient = gethostbyaddr((char*)&client.sin_addr, sizeof(client.sin_addr), AF_INET);
                std::cout << "Client hostname: " << hostnameClient->h_name << std::endl;

                Server::PutAnswerToClient(&serverSocket, name, (sockaddr*)&client, &lc);
            }
        }

        if (closesocket(serverSocket) == SOCKET_ERROR) {
            throw SocketError::SetErrorMsgText("closesocket:", WSAGetLastError());
        }
        if (WSACleanup() == SOCKET_ERROR) {
            throw SocketError::SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
    }
    catch (std::string errorMsgText) {
        std::cout << std::endl << errorMsgText;
    }
    system("pause");
    return 0;
}
