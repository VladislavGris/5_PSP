#include <iostream>
#include <Windows.h>
#define NAME L"\\\\.\\mailslot\\Box"

int main()
{
    setlocale(LC_ALL, "rus");

    HANDLE sH;
    DWORD rb, time = 1800000, nMaxMessageSize = 1000;
    clock_t start, end;
    char rbuf[300];
    bool flag = true;

    try {
        std::cout << "ServerMS" << std::endl;

        if ((sH = CreateMailslot(NAME, nMaxMessageSize, time, NULL)) == INVALID_HANDLE_VALUE) {
            throw GetLastError();
        }
        while (true) {
            std::cout << "Waiting for connections..." << std::endl;
            if (!ReadFile(sH, rbuf, sizeof(rbuf), &rb, NULL)) {
                throw GetLastError();
            }
            else {
                if (flag) {
                    start = clock();
                    flag = false;
                }
            }
            if (strcmp(rbuf, "STOP") == 0)
            {
                end = clock();
                std::cout << "Elapsed time : " << ((double)(end - start) / CLK_TCK) << " c" << std::endl;
                flag = true;
            }
            std::cout << rbuf << std::endl;
        }
        if (!CloseHandle(sH)) {
            throw GetLastError();
        }

        system("pause");
    }
    catch (std::string ErrorPipeText) {
        std::cout << std::endl << ErrorPipeText;
    }
}
