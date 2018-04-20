#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <stack>

using namespace std;
#define MAX 10

#pragma warning(disable:4996)
using namespace std;

stack<HANDLE> Threads;
CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(LPVOID lp_param)
{
	while (1)
	{
		//if (TryEnterCriticalSection(&CriticalSection) != 0)
		//{
			char string[30];
			sprintf(string, "Thread %d is running", (int)lp_param);
			EnterCriticalSection(&CriticalSection);
			for (int i = 0; i < strlen(string); i++)
			{
				//cout << string[i];
				Sleep(7);
				putchar(string[i]);
			}
			putchar('\n');
			LeaveCriticalSection(&CriticalSection);
			Sleep(3);
		//}
	}
	return 0;
}



int main(int argc, char* argv[])
{
	cout << "Choices: " << endl;
	cout << "* write '+', if you want to add new process" << endl;
	cout << "* write '-', if you want to delete last process" << endl;
	cout << "* write 'q', if you want to quit" << endl;

	InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);

	while (1)
	{
		char choice;
		choice = _getch();
		switch (choice)
		{
		case '+':
			if (Threads.size() < MAX)
			{
				HANDLE hThread;
				DWORD dwThreadIdArray;

				hThread = CreateThread(
					NULL,                   // default security attributes
					0,                      // use default stack size  
											//(LPTHREAD_START_ROUTINE)ThreadProc,       // thread function name
					ThreadProc,
					(void*)Threads.size(),          // argument to thread function 
					0,                      // use default creation flags 
					&dwThreadIdArray);

				if (hThread == INVALID_HANDLE_VALUE)
				{
					cout << "CreateThread error\n" << endl;
					system("pause");
					return 0;
				}
				Threads.push(hThread);
				//std::cout << "\n Count of processes: " << Threads.size() << std::endl;
			}
			else
			{
				cout << "\n Max 10 " << endl;
			}
			break;
		case'-':
			if (!Threads.empty())
			{
				EnterCriticalSection(&CriticalSection);
				TerminateThread(Threads.top(), NO_ERROR);
				CloseHandle(Threads.top());
				Threads.pop();

				if (Threads.size() == 0)
				{
					InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
				}
				LeaveCriticalSection(&CriticalSection);
			}
			else
			{
				cout << "\n Stack is empty " << endl;
			}
			break;
		case'q':
			EnterCriticalSection(&CriticalSection);
			//while (!Threads.empty())
			//{

				while (!Threads.empty())
				{
					TerminateProcess(Threads.top(), NO_ERROR);
					CloseHandle(Threads.top());
					Threads.pop();
					cout << "\n Count of processes: " << Threads.size() << endl;
				}
				LeaveCriticalSection(&CriticalSection);
				DeleteCriticalSection(&CriticalSection);
				return 0;
			//}
			break;
		default:
			break;
		}
	}
}
