#include "../SocketError.h"
#include "../Server/ServerFunctions.h"
#define PORT 2000
#define NAME "DESKTOP-LRSVKUO"
#define CALL "Hello"

int main()
{
    WSADATA wsaData;
    SOCKADDR_IN clnt;
    int lc = sizeof(clnt);

    try {

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw  SocketError::SetErrorMsgText("Startup: ", WSAGetLastError());
        }

        Server::GetServerByName(NAME, CALL, (sockaddr*)&clnt,PORT, &lc);

        if (WSACleanup() == SOCKET_ERROR) {
            throw  SocketError::SetErrorMsgText("Cleanup: ", WSAGetLastError());
        }
    }
    catch (std::string errorMsgText) {
        std::cout << std::endl << errorMsgText;
    }

    system("pause");
    return 0;
}
