#include <stdio.h>
#include <tchar.h>
#include <windows.h>

DWORD WINAPI WaitThread(LPVOID *pParam)
{
	getchar();

	return 0;
}

int main(int argc, char *_argv[])
{
#ifdef _UNICODE
	WCHAR **argv = CommandLineToArgvW(GetCommandLineW(), &argc);
#else
	char **argv = _argv;
#endif

	if (argc < 2) {
		fprintf(stderr, "Missing time");

		return 1;
	}

#ifdef _UNICODE
	TCHAR *pEnd;
	LONGLONG sec = wcstoll(argv[1], &pEnd, 10);
#else
	char *pEnd;
	LONGLONG sec = strtoll(argv[1], &pEnd, 10);
#endif

	if (errno == 34) {
		fprintf(stderr, "Invalid time");

		return 1;
	}

	LARGE_INTEGER time;
	time.QuadPart = sec * 10000000;

	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, argc > 2 ? argv[2] : TEXT("ResumeTimer"));

	if (!hTimer || GetLastError()) {
		fprintf(stderr, "Could not create timer");

		return 1;
	}

	if (!SetWaitableTimer(hTimer, &time, 0, NULL, NULL, TRUE) || GetLastError()) {
		fprintf(stderr, "Could not set timer");

		return 1;
	}

	HANDLE hWaitThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitThread, NULL, 0, NULL);

	while (WaitForSingleObject(hWaitThread, 10UL) != WAIT_OBJECT_0 && WaitForSingleObject(hTimer, 10UL) == WAIT_TIMEOUT);
	
	SetThreadExecutionState(ES_SYSTEM_REQUIRED);
	
	if (hTimer) {
		CancelWaitableTimer(hTimer);
		CloseHandle(hTimer);
		hTimer = NULL;
	}

	if (hWaitThread) {
		TerminateThread(hWaitThread, -1);
		CloseHandle(hWaitThread);
		hWaitThread = NULL;
	}
}
