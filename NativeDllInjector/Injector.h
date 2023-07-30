#ifndef INJECTOR_H
#define INJECOTR_H

#define RTN_OK		0
#define RTN_USAGE	1
#define RTN_ERROR	13

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <tlHelp32.h>
#include <string.h>

#define DEBUG_NTBUFFER

#ifdef DEBUG_NTBUFFER


// Unknown buffer structuew definition in NtCreateThread API
struct NtCreateThreadExBuffer
{
	SIZE_T	Size;
	SIZE_T	Unknown1;
	SIZE_T	Unknown2;
	PULONG	Unknown3;
	SIZE_T	Unknown4;
	SIZE_T	Unknown5;
	SIZE_T	Unknown6;
	PULONG	Unknown7;
	SIZE_T	Unknown8;
};

#endif


// NtCreateThreadEx header definition based on https://undocumented.ntinternals.net/
typedef NTSTATUS(WINAPI* lpNtCreateThreadEx)(

	OUT		PHANDLE				hThread,
	IN		ACCESS_MASK			DesiredAccess,
	IN		LPVOID				ObjectAttributes,
	IN		HANDLE				ProcessHandle,
	IN		LPVOID				lpStartAddress,
	IN		LPVOID				lpParameter,
	IN		ULONG				CreateSuspended,
	IN		SIZE_T				StackZeroBits,
	IN		SIZE_T				SizeOfStackCommit,
	IN		SIZE_T				SizeOfStackReserve,
	OUT		LPVOID				lpBytesBuffer
	);


// tech 5 ---> RtlCreatUserThread
BOOL RtlCreateUserThread(HANDLE hProcess, LPCSTR DllPath);


// RtlCreateUserThread header definition based on https://undocumented.ntinternals.net/
typedef DWORD(WINAPI* pRtlCreatUserThread)(

	IN		HANDLE					ProcessHandle,
	IN 		PSECURITY_DESCRIPTOR	SecurityDescriptor,
	IN		BOOLEAN					CreateSuspended,
	IN		ULONG					StackZeroBits,
	IN OUT	PULONG					StackReserved,
	IN OUT	PULONG					StackCommit,
	IN		PVOID					StartAddress,
	IN		PVOID					StartParameter,
	OUT		PHANDLE					ThreadHandle,
	OUT		PVOID					ClientID

	);

#endif