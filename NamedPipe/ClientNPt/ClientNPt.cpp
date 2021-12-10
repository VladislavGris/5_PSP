#include <iostream>
#include <string>
#include <Windows.h>
#define NAME L"\\\\.\\pipe\\Tube"
#define STOP "STOP"

int main()
{
    HANDLE cH;
    DWORD lp, mode = PIPE_READMODE_MESSAGE;
    char ibuf[50], obuf[50];

    try {
        std::cout << "ClientNP" << std::endl;

        if ((cH = CreateFile(NAME, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE) {
            throw  GetLastError();
        }
        if (!SetNamedPipeHandleState(cH, &mode, NULL, NULL)) {
            throw  GetLastError();
        }

        int countMessage;
        std::cout << "Message count: ";
        std::cin >> countMessage;

        for (int i = 1; i <= countMessage; i++) {
            std::string obufstr = "Hello from ClientNPt " + std::to_string(i);
            strcpy_s(obuf, obufstr.c_str());

            if (!TransactNamedPipe(cH, obuf, sizeof(obuf), ibuf, sizeof(ibuf), &lp, NULL)) {
                throw GetLastError();
            }

            std::cout << ibuf << std::endl;
        }

        if (!WriteFile(cH, STOP, sizeof(STOP), &lp, NULL)) {
            throw GetLastError();
        }
        if (!CloseHandle(cH)) {
            throw GetLastError();
        }

        system("pause");
    }
    catch (std::string ErrorPipeText) {
        std::cout << ErrorPipeText << std::endl;
    }
}
