#include <stdio.h>
#include <conio.h>
#include <windows.h>

typedef void(*SetHookProc)(HHOOK);
const char *title = "��ɽ����ͨ 2016";
char errorText[256] = {0};

#define FailLog(T) printf("[ʧ��]\n");\
	MessageBox(NULL, T, "����", MB_OK|MB_ICONERROR);
#define ErrorTextLog FailLog(errorText);

HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.
	// Fetch current window title.
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
	// Format a "unique" NewWindowTitle.
	wsprintf(pszNewWindowTitle,"%d/%d",
			GetTickCount(),
			GetCurrentProcessId());
	// Change current window title.
	SetConsoleTitle(pszNewWindowTitle);
	// Ensure window title has been updated.
	Sleep(40);
	// Look for NewWindowTitle.
	hwndFound=FindWindow(NULL, pszNewWindowTitle);
	// Restore original window title.
	SetConsoleTitle(pszOldWindowTitle);
	return(hwndFound);
}

HHOOK hook = NULL;

void whenExit()
{
	if(hook != NULL){
		UnhookWindowsHookEx(hook);
	}
}

int main()
{
	SetConsoleTitle("Easy Type v1.0 by iTruth");
	atexit(whenExit);
	printf("���ڼ���hookdll_utf8_x32.dll...");
	HMODULE dll = LoadLibrary("hookdll_utf8_x32.dll");
	if(dll == NULL){
		sprintf(errorText, "��̬���ӿ�hookdll_utf8_x32.dll��ʧ\n�������: %ld", GetLastError());
		ErrorTextLog;
		return -1;
	}
	printf("[���]\n");
	HOOKPROC hookCallBack = (HOOKPROC)GetProcAddress(dll, "funcCallback");
	SetHookProc setHook = (SetHookProc)GetProcAddress(dll, "setHook");

	printf("���ڻ�ȡ[%s]�Ĵ��ھ��...", title);
	HWND hWndDest = FindWindow(NULL, title);
	if(hWndDest == NULL){
		sprintf(errorText, "�޷���ȡ���ھ��,������[%s]��δ�������߰汾����", title);
		ErrorTextLog;
		return 0;
	}
	printf("[���]\n");

	DWORD dwThreadDest = GetWindowThreadProcessId(hWndDest, NULL);
	printf("������Ŀ���߳�ע�빳��...");
	hook = SetWindowsHookEx(WH_GETMESSAGE, hookCallBack, dll, dwThreadDest);
	if(hook == NULL){
		sprintf(errorText, "ע�빳��ʧ��,���Ժ�����");
		ErrorTextLog;
		return -1;
	}
	printf("[���]\n");
	setHook(hook);
	printf("����ע��ɹ�!\n\n�ڱ����ڰ�������������Ӳ��˳�\n");
	ShowWindow(GetConsoleHwnd(), SW_SHOWMINIMIZED);
	(void)getch();
	return 0;
}
