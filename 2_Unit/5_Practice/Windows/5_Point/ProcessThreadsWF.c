#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD pid;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//Creación proceso hijo
	if(!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("Error: cannot invoke CreateProcess(%d)\n", GetLastError());
		return 0;
	}

	//Proceso padre
	pid = GetCurrentProcessId();
	printf("I'm the father my pid is: %d\n", pid);
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}

