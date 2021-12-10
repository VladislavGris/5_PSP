#include <iostream>
#include <Windows.h>
#define NAME L"\\\\.\\pipe\\Tube"
#define STOP "STOP"

int main()
{
    HANDLE sH;
    DWORD lp;
    char buf[50];

    try {
        std::cout << "ServerNP" << std::endl;

        if ((sH = CreateNamedPipe(NAME,
            PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT,
            1, NULL, NULL, INFINITE, NULL)) == INVALID_HANDLE_VALUE) {
            throw GetLastError();
        }
        
        while (true) {
            std::cout << "Waiting for connections..." << std::endl;
            if (!ConnectNamedPipe(sH, NULL)) {
                throw GetLastError();
            }
            while (true) {
                if (ReadFile(sH, buf, sizeof(buf), &lp, NULL)) {

                    if (strcmp(buf, STOP) == 0) {
                        std::cout << std::endl;
                        break;
                    }
                    std::cout << buf << std::endl;
                    if (WriteFile(sH, buf, sizeof(buf), &lp, NULL)) {
                        if (strstr(buf, "ClientNPct")) {
                            break;
                        }
                    }
                    else {
                        throw GetLastError();
                    }
                }
                else {
                    throw GetLastError();
                }
            }
            if (!DisconnectNamedPipe(sH)) {
                throw GetLastError();
            }
        }

        if (!CloseHandle(sH)) {
            throw GetLastError();
        }

        system("pause");
    }
    catch (std::string ErrorPipeText) {
        std::cout << ErrorPipeText << std::endl;
    }
}
