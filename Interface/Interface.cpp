// Interface.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>

#define print(...) printf(__VA_ARGS__); printf("\n"); // don't do this guys

int main()
{
    HANDLE bozopipe = CreateFile(
        L"\\\\.\\pipe\\bozo",
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (bozopipe == INVALID_HANDLE_VALUE)
    {
        print("CreateFile failed");
        return 1;
    }

    std::string input;
    while (true)
    {
        std::cout << "Input << ";
        std::getline(std::cin, input);
        if (input == "exit")
            break;

        DWORD dwWritten;
        BOOL success = WriteFile(
            bozopipe,
            input.c_str(),
            (DWORD)(static_cast<unsigned long long>(input.length()) + 1) * sizeof(char),
            &dwWritten,
            NULL);

        if (!success || dwWritten == 0)
        {
            print("WriteFile failed");
        }
        else {
            if (!FlushFileBuffers(bozopipe)) {
                std::cerr << "FlushFileBuffers failed" << std::endl;
            }
        }
    }

    CloseHandle(bozopipe);

    return 0;
}
