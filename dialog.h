#ifndef __DIALOG_H
#define __DIALOG_H
#include<windows.h>

INT_PTR WINAPI
DialogFunc(
	HWND hDlg,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);

VOID
SetConfigOptions(
	HWND hDlg,
	PVOID ConfigBuffer
);

PVOID
SaveConfigOptions(
	PVOID ConfigBuffer
);

#endif // __DIALOG_H