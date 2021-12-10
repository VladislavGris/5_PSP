#include "ServerFunctions.h"

namespace Server {

	bool GetRequestFromClient(SOCKET* ss, char* name, short port, struct sockaddr* from, int* flen) {
        char ibuf[10];
        int  lb = 0;
        try {
            std::cout << "Waiting for client..." << std::endl;
            while (true) {
                if ((lb = recvfrom(*ss, ibuf, sizeof(ibuf), NULL, from, flen)) == SOCKET_ERROR) {
                    throw  SocketError::SetErrorMsgText("recvfrom:", WSAGetLastError());
                }
                if (strcmp(name, ibuf) == 0) {
                    struct sockaddr_in* client = (struct sockaddr_in*)from;
                    std::cout << "Client IP: " << inet_ntoa(client->sin_addr) << std::endl;

                    return true;
                }
            }
        }
        catch (int errCode) {
            return errCode == WSAETIMEDOUT ? false : throw  SocketError::SetErrorMsgText("recvfrom:", WSAGetLastError());
        }
	}

	bool PutAnswerToClient(SOCKET* ss, char* name, struct sockaddr* to, int* lto) {
        try {
            if ((sendto(*ss, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR) {
                throw  SocketError::SetErrorMsgText("send:", WSAGetLastError());
            }
        }
        catch (std::string errorMsgText) {
            std::cout << std::endl << "WSAGetLastError: " << errorMsgText;
            return false;
        }

        return true;
	}

    bool GetServer(char* call, short port, struct sockaddr* from, int* flen) {
        SOCKET cC;
        char buf[50];
        int countServers = 0;

        try {
            int optval = 1, lb = 0, lobuf = 0;

            SOCKADDR_IN all;
            all.sin_family = AF_INET;
            all.sin_port = htons(port);
            all.sin_addr.s_addr = INADDR_BROADCAST;

            timeval timeout;
            timeout.tv_sec = 10000;
            timeout.tv_usec = 0;

            if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
                throw  SocketError::SetErrorMsgText("socket:", WSAGetLastError());
            }
            if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) {
                throw  SocketError::SetErrorMsgText("setsocketopt:", WSAGetLastError());
            }
            if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout))) {
                throw  SocketError::SetErrorMsgText("setsocketopt:", WSAGetLastError());
            }
            if ((lb = sendto(cC, call, sizeof(call) + 2, NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR) {
                throw SocketError::SetErrorMsgText("sendto:", WSAGetLastError());
            }
            std::cout << "Waiting for servers..." << std::endl;
            while (true)
            {
                if (recvfrom(cC, buf, sizeof(buf), NULL, from, flen) == SOCKET_ERROR) {
                    throw  SocketError::SetErrorMsgText("recvfrom:", WSAGetLastError());
                }
                if (strcmp(call, buf) == 0) {
                    std::cout << "Server IP: " << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << std::endl;
                    countServers++;
                }
                else {
                    std::cout << "Incorrect name: " << buf << std::endl;
                    return false;
                }
            }

        }
        catch (std::string errorMsgText)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) {
                std::cout << "Find servers: " << countServers << std::endl;
                if (closesocket(cC) == SOCKET_ERROR) {
                    throw SocketError::SetErrorMsgText("closesocket: ", WSAGetLastError());
                }
            }
            else {
                throw errorMsgText;
            }
        }
        return true;
    }

    bool GetServerByName(const char* name, const char* call, struct sockaddr* from, short port, int* flen) {
        hostent* host = gethostbyname(name);

        try {
            SOCKET cC;
            int optval = 1, lb = 0, lobuf = 0;
            char buf[50];
            in_addr* addr_list = (in_addr*)*(host->h_addr_list);

            SOCKADDR_IN all;
            all.sin_family = AF_INET;
            all.sin_port = htons(port); 
            all.sin_addr = *addr_list;

            if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
                throw  SocketError::SetErrorMsgText("socket:", WSAGetLastError());
            }
            if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) {
                throw  SocketError::SetErrorMsgText("setsocketopt:", WSAGetLastError());
            }
            if ((lb = sendto(cC, call, sizeof(call) + 2, NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR) {
                throw SocketError::SetErrorMsgText("sendto:", WSAGetLastError());
            }
            if (lobuf = recvfrom(cC, buf, sizeof(buf), NULL, (sockaddr*)from, flen) == SOCKET_ERROR) {
                throw SocketError::SetErrorMsgText("recvfrom:", WSAGetLastError());
            }

            struct sockaddr_in* server = (struct sockaddr_in*)from;
            std::cout << "Server port: " << ntohs(server->sin_port) << std::endl;
            std::cout << "Server IP: " << inet_ntoa(*addr_list) << std::endl;

            if (closesocket(cC) == SOCKET_ERROR) {
                throw SocketError::SetErrorMsgText("closesocket: ", WSAGetLastError());
            }

            return strcmp(call, buf) == 0 ? true : false;
        }
        catch (int errCode) {
            return errCode == WSAETIMEDOUT ? false : throw  SocketError::SetErrorMsgText("recvfrom:", WSAGetLastError());
        }
    }
}