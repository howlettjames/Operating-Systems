#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI thread(LPVOID arg);
DWORD WINAPI thread1(LPVOID arg);
DWORD WINAPI thread2(LPVOID arg);

int main(void)
{
	DWORD pid, id_thread;
	HANDLE han_thread;
	int *no, j;

	pid = GetCurrentProcessId();
	printf("I'm the son my pid is: %d\n", pid);
	for(int i = 0; i < 15; i++)
	{
		//*no = i + 1;
		j = i + 1;
		han_thread = CreateThread(NULL, 0, thread, &j, 0, &id_thread);
		WaitForSingleObject(han_thread, INFINITE);
	}
		
	return 0;
}

DWORD WINAPI thread(LPVOID arg)
{
	int *no = arg, j;
	DWORD id_thread = GetCurrentProcessId();
	HANDLE han_thread;

	printf("\tI'm thread %d my pid is: %ld\n", *no, id_thread);

	for(int i = 0; i < 10; i++)
	{
		//*no = i + 1;
		j = i + 1;
		han_thread = CreateThread(NULL, 0, thread1, &j, 0, &id_thread);
		WaitForSingleObject(han_thread, INFINITE);
	}
	return 0;
}

DWORD WINAPI thread1(LPVOID arg)
{
	int *no = arg, j;
	DWORD id_thread = GetCurrentProcessId();
	HANDLE han_thread;

	printf("\t\tI'm thread %d my pid is: %ld\n", *no, id_thread);

	for(int i = 0; i < 5; i++)
	{
		//*no = i + 1;
		j = i + 1;
		han_thread = CreateThread(NULL, 0, thread2, &j, 0, &id_thread);
		WaitForSingleObject(han_thread, INFINITE);
	}
	return 0;
}

DWORD WINAPI thread2(LPVOID arg)
{
	int *no = arg;
	DWORD id_thread = GetCurrentProcessId();
	printf("\t\t\tI'm thread %d Practica 5\n", *no);

	return 0;
}