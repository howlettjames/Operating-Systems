#include <stdio.h>
#include <windows.h>

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int i = 0;
	char *argv[] = {"5a1", "5a2", "5a3"};
	DWORD pid = GetCurrentProcessId();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	printf("\t\tI'm 5a | ID: %d\n", pid);	
	for(i = 0; i < 3; i++)
		if(!CreateProcess(NULL, argv[i], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Error: failed to invoke CreateProcess(%d)\n", GetLastError());
			return 0;
		}

	for(i = 0; i < 3; i++)
		WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}