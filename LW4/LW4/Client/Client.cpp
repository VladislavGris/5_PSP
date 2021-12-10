#include "../Server/ServerFunctions.h"
#include "../SocketError.h"
#define PORT 2000

int main()
{
    WSADATA wsaData;

    char name[10] = "Hello";

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw  SocketError::SetErrorMsgText("Startup: ", WSAGetLastError());
        }

        SOCKADDR_IN clnt;
        int lc = sizeof(clnt);

        Server::GetServer(name, PORT, (LPSOCKADDR)&clnt, &lc);

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
