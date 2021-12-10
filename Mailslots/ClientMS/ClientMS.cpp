#include <iostream>
#include <Windows.h>
#define NAME L"\\\\.\\mailslot\\Box"
#define STOP "STOP"

int main()
{
    HANDLE cM;
    DWORD rb;
    clock_t start, end;
    char wbuf[300] = "Hello from Maislot-client";

    try {
        std::cout << "ClientMS"<<std::endl;

        if ((cM = CreateFile(NAME, GENERIC_WRITE, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE) {
            throw GetLastError();
        }

        int countMessage;
        std::cout << "Number of messages: ";
        std::cin >> countMessage;

        for (int i = 1; i <= countMessage; i++) {
            if (i) {

                start = clock();
            }

            if (!WriteFile(cM, wbuf, sizeof(wbuf), &rb, NULL)) {
                throw GetLastError();
            }

            std::cout << wbuf << std::endl;
        }

        if (!WriteFile(cM, STOP, sizeof(STOP), &rb, NULL)) {
            throw GetLastError();
        }

        end = clock();
        std::cout << "Elapsed time: " << ((double)(end - start) / CLK_TCK) << " c" << std::endl;


        if (!CloseHandle(cM)) {
            throw GetLastError();
        }

        system("pause");
    }
    catch (std::string ErrorPipeText) {
        std::cout << std::endl << ErrorPipeText;
    }
}
