#include <Windows.h>
#include <stdio.h>
#include "Injector.h"

int main(int argc, char* argv[]) {
    LPCSTR dllPath = NULL;
    DWORD processID = 0;
    HANDLE hTargetProcess = NULL;

    if (argc != 3) {
        printf("Usage: %s <path_to_dll> <process_id>\n", argv[0]);
        return 1;
    }

    dllPath = argv[1];
    processID = (DWORD)atoi(argv[2]);

    printf("DLL Path: %s\n", dllPath);
    printf("Process ID: %u\n", processID);

    hTargetProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, processID);

    RtlCreateUserThread(hTargetProcess, dllPath);

    return 0;
}
