#include "Injector.h"

BOOL RtlCreateUserThread(HANDLE hProcess, LPCSTR DllPath) {

	LPVOID LoadLibraryAddr = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	if (!LoadLibraryAddr) {
		printf("Could note get real address of LoadLibraryA from kernel32.dll!\n");
		printf("LastError : 0X%x\n", GetLastError());
		return false;
	}

	printf("LoadLibraryA is located at real address: 0X%p\n", (void*)LoadLibraryAddr);

	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!pDllPath) {
		printf("Could not allocate Memory in target process\n");
		printf("LastError : 0X%x\n", GetLastError());
		return false;
	}

	printf("Dll path memory allocated at: 0X%p\n", (void*)pDllPath);

	BOOL written = WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath, strlen(DllPath), NULL);

	if (!written) {
		printf("Could not write into the allocated memory\n");
		printf("LastError : 0X%x\n", GetLastError());
		return false;
	}

	printf("Dll path memory was written at address : 0x%p\n", (void*)pDllPath);

	HMODULE modNtDll = GetModuleHandle(L"ntdll.dll");

	if (!modNtDll) {
		printf("Failed to get module handle for ntdll.dll\n");
		printf("LastError : 0X%x\n", GetLastError());
		return false;
	}

	pRtlCreatUserThread pfunc_RtlCreateUserThread = (pRtlCreatUserThread)GetProcAddress(modNtDll, "RtlCreateUserThread");

	if (!pfunc_RtlCreateUserThread) {
		printf("Failed to get RtlCreateThreadEx function address from ntdll.dll\n");
		printf("LastError: 0X%x\n", GetLastError());
		return false;
	}

	HANDLE hThread = NULL;

	pfunc_RtlCreateUserThread(
		hProcess,
		NULL,
		0,
		0,
		0,
		0,
		LoadLibraryAddr,
		pDllPath,
		&hThread,
		NULL
	);

	if (!hThread) {

		printf("\nRtlCreateUserThreadEx failed\n");
		printf("LastError: 0X%x\n", GetLastError());

		if (VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE)) {
			printf("Memory was freed in target process\n");
		}
		return false;
	}

	printf("Thread started with RtlCreateUserThread\n");

	WaitForSingleObject(hThread, INFINITE);

	if (VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE)) {
		//VirtualFreeEx(hProc, reinterpret_cast<int*>(pDllPath) + 0X010000, 0, MEM_RELEASE);
		printf("Memory was freed in target process\n");
	}

	CloseHandle(hThread);

	CloseHandle(hProcess);

	return true;
}