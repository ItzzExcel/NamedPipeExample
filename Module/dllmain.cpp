// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>
#include <iostream>
#include <string>

#define print(...) printf(__VA_ARGS__); printf("\n"); // don't do this guys

char buffer[0x2FAF0800] = ""; // don't do this guys

void Read() {
    while (true) {
        HANDLE hPipe = CreateNamedPipe(
            L"\\\\.\\pipe\\bozo",
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            0x2FAF0800, // don't do this guys
            0x2FAF0800, // don't do this guys
            0,
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            print("CreateNamedPipe failed");
            return;
        }

        if (ConnectNamedPipe(hPipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED)
        {
            print("Pipe connected, ready");
            DWORD dwRead;

            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                buffer[dwRead] = '\0';
                std::string received(buffer, dwRead);
                print(">> %s", received.c_str());
            }

            print("Disconnected");

            FlushFileBuffers(hPipe);
        }
        else
        {
            print("Connection failed");
        }

        CloseHandle(hPipe);
    }
}

int main()
{
    if (AllocConsole())
    {
        FILE* stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
    }
    Read();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
