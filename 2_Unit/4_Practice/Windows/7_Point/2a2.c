#include <stdio.h>
#include <windows.h>

int main(void)
{
	DWORD pid = GetCurrentProcessId();
	printf("\t\t\tI'm 2a2 | ID: %d\n", pid);
	return 0;
}