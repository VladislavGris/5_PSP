#include <iostream>
#include <string>
#include <Windows.h>
#define NAME L"\\\\.\\pipe\\Tube"

int main()
{
    HANDLE cH;
    DWORD lp, mode = PIPE_READMODE_MESSAGE;
    char ibuf[50], obuf[50];

    try {
        std::cout << "ClientNP" << std::endl;

        int countMessage;
        std::cout << "Message count: ";
        std::cin >> countMessage;

        for (int i = 1; i <= countMessage; i++) {
            std::string obufstr = "Hello from ClientNPct" + std::to_string(i);
            strcpy_s(obuf, obufstr.c_str());
            if (!CallNamedPipe(NAME, obuf, sizeof(obuf), ibuf, sizeof(ibuf), &lp, NMPWAIT_WAIT_FOREVER)) {
                throw GetLastError();
            }
            std::cout << ibuf << std::endl;
        }
        system("pause");
    }
    catch (std::string ErrorPipeText) {
        std::cout << ErrorPipeText << std::endl;
    }
}