#include <stdio.h>
#include <windows.h>

int main(void)
{
	DWORD pid = GetCurrentProcessId();
	printf("\t\t\tI'm 1a3 | ID: %d\n", pid);
	return 0;
}