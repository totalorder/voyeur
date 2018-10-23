#include <stdio.h>
#include <windows.h>
#include "common.h"
/**
 * In the following registry key create a String Value with the full path of main.exe:
 * HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\child.exe
 */
int main(int argc, char *argv[]) {
    FILE* file;
    file = fopen("main.txt", "w");

    dumpArgs(file, argc, argv);

    PROCESS_INFORMATION processInformation;
    STARTUPINFO startupInfo = { sizeof(startupInfo) };
    char commandLine[MAX_PATH];
    // TODO: Include the rest of the args
    lstrcpyA(commandLine, argv[1]);

    output(file, "\n");
    output(file, "Creating process: %s\n", &commandLine);
    BOOL created = CreateProcess(NULL, commandLine, NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL, &startupInfo, &processInformation);
    if (!created) {
        output(file, "Failed to create process: %s\n", commandLine);

        return 1;
    }

    output(file, "Process created: %d\n", processInformation.dwProcessId);
    output(file, "Listening for events...\n");

    DEBUG_EVENT debugEvent;
    DWORD continueStatus = DBG_CONTINUE;

    int running = 1;
    while (running) {
        if (!WaitForDebugEvent(&debugEvent, INFINITE) ) {
            printf("asd %lu", processInformation.dwProcessId);
            output(file, "No debug event %lu!\n", processInformation.dwProcessId);
            return 1;
        }

        switch (debugEvent.dwDebugEventCode) {
            case CREATE_PROCESS_DEBUG_EVENT:
                output(file, "Received CREATE_PROCESS_DEBUG_EVENT\n", NULL);
                if (debugEvent.u.CreateProcessInfo.hFile) {
                    output(file, "Closing create process file handle: %d\n", debugEvent.u.CreateProcessInfo.hFile);
                    CloseHandle(debugEvent.u.CreateProcessInfo.hFile);
                }
                break;
            case LOAD_DLL_DEBUG_EVENT:
                output(file, "Received LOAD_DLL_DEBUG_EVENT\n");
                break;
            case UNLOAD_DLL_DEBUG_EVENT:
                output(file, "Received UNLOAD_DLL_DEBUG_EVENT\n");
                break;
            case CREATE_THREAD_DEBUG_EVENT:
                output(file, "Received CREATE_THREAD_DEBUG_EVENT\n");
                break;
            case EXCEPTION_DEBUG_EVENT:
                output(file, "Received EXCEPTION_DEBUG_EVENT\n");
                break;
            case EXIT_THREAD_DEBUG_EVENT:
                output(file, "Received EXIT_THREAD_DEBUG_EVENT\n");
                break;
            case EXIT_PROCESS_DEBUG_EVENT:
                output(file, "Received EXIT_PROCESS_DEBUG_EVENT\n");
                running = 0;
                break;
            default:
                output(file, "Received unknown debug event: %d", debugEvent.dwDebugEventCode);
                return 1;
        }

        if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, continueStatus)) {
            output(file, "Failed to continue debug event: %d\n", processInformation.dwProcessId);
            return 1;
        };
    }

    output(file, "Waiting for process %d to exit...\n", processInformation.hProcess);

    DWORD exitCode;
    WaitForSingleObject(processInformation.hProcess, 1000);
    BOOL exited = GetExitCodeProcess(processInformation.hProcess, &exitCode);
    if (!exited) {
        output(file, "No exit code received for pid %d\n", processInformation.hProcess);
        return 1;
    }

    output(file, "Process exited with code: %d", exitCode);

    fclose(file);
    return 0;
}

