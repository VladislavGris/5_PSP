#include <iostream>
#include <string>
#include <Windows.h>
#define NAME L"\\\\.\\pipe\\Tube"
#define STOP "STOP"

int main()
{
    HANDLE cH;
    DWORD lp;
    char buf[50];

    try {
        std::cout << "ClientNP" << std::endl;

        if ((cH = CreateFile(NAME, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE) {
            throw  GetLastError();
        }

        int countMessage;
        std::cout << "Message count: ";
        std::cin >> countMessage;

        for (int i = 1; i <= countMessage; i++) {
            std::string obuf = "Hello from ClientNP " + std::to_string(i);

            if (!WriteFile(cH, obuf.c_str(), sizeof(obuf), &lp, NULL)) {
                throw GetLastError();
            }
            if (!ReadFile(cH, buf, sizeof(buf), &lp, NULL)) {
                throw GetLastError();
            }

            std::cout << buf << std::endl;
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
