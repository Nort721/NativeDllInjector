#include <Windows.h>
#include <stdio.h>

int main() {
	while (1) {
		printf("process id: %d\n", GetCurrentProcessId());
		Sleep(1000);
	}
	return 1;
}
