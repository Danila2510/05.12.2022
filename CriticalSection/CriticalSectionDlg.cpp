#include "CriticalSectionDlg.h"

CriticalSectionDlg* CriticalSectionDlg::ptr = NULL;

CriticalSectionDlg::CriticalSectionDlg(void)
{
	ptr = this;
}

CriticalSectionDlg::~CriticalSectionDlg(void)
{

}

void CriticalSectionDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CriticalSectionDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	return TRUE;
}

DWORD WINAPI Write(LPVOID lp)
{
	srand(time(0));
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{52767BF0 - E85A - 4672 - 9377 - D711AC6C949A}"));
	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		ofstream out(TEXT("array.txt"));
		if (!out.is_open())
		{
			MessageBox(0, TEXT("Error"), TEXT("Critiale section"), MB_OK);
			return 1;
		}
		int A[100];
		for (int i = 0; i < 100; i++)
		{
			A[i] = rand() % 50;
			out << A[i] << ' ';
		}
		out.close();
	}
	ResetEvent(hEvent);
	MessageBox(0, TEXT("Potok write information in file"), TEXT("Critiale section"), MB_OK);
	return 0;
}

DWORD WINAPI Read(LPVOID lp)
{
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{52767BF0 - E85A - 4672 - 9377 - D711AC6C949A}"));
	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		ifstream in(TEXT("array.txt"));
		if (!in.is_open())
		{
			MessageBox(0, TEXT("Error"), TEXT("Error"), MB_OK);
			return 1;
		}
		int B[100];
		int sum = 0;
		for (int i = 0; i < 100; i++)
		{
			in >> B[i];
			sum += B[i];
		}
		in.close();
	}
	ResetEvent(hEvent);
	MessageBox(0, TEXT("Potor write information in file"), TEXT("Critiale section"), MB_OK);
	TCHAR str[30];
	MessageBox(0, str, TEXT("Critiale section"), MB_OK);
	return 0;
}

void CriticalSectionDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_BUTTON1)
	{
		CreateEvent(NULL, TRUE, FALSE, TEXT("{52767BF0 - E85A - 4672 - 9377 - D711AC6C949A}"));
		HANDLE hThread = CreateThread(NULL, 0, Write, 0, 0, NULL); 
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, Read, 0, 0, NULL);  
		CloseHandle(hThread);
	}
}

BOOL CALLBACK CriticalSectionDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}