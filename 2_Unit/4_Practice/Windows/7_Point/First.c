#include <stdio.h>
#include <windows.h>

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int i = 0;
	char *argv[] = {"1a", "2a", "3a", "4a", "5a"};
	DWORD pid = GetCurrentProcessId();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	printf("\tI'm the main son | ID: %d\n", pid);	
	for(i = 0; i < 5; i++)
	{
		if(!CreateProcess(NULL, argv[i], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Error: failed to invoke CreateProcess(%d)\n", GetLastError());
			return 0;
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	
	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}