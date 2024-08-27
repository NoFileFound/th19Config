#include <Windows.h>
#include "winapi.h"
#include "resource.h"
#include "dialog.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	LCID locale = GetUserDefaultLCID();
	DWORD dialogValue;
	switch (locale) {
		case LOCALE_JAPAN_LANG:
			dialogValue = MAIN_DIALOG_JAPANESE_LANG;
			break;
		case LOCALE_BRAZILIAN_LANG:
			dialogValue = MAIN_DIALOG_BRAZILIAN_LANG;
			break;
		default:
			dialogValue = MAIN_DIALOG_ENGLISH_LANG;
			break;
	}

	INT_PTR dialog = DialogBoxParamA(hInstance, (LPCSTR)(dialogValue), 0, DialogFunc, 0);
	if (dialog == -1) {
        char fileName[MAX_PATH];
        GetCurrentProgramName(fileName, MAX_PATH);
        MessageBoxA(GetForegroundWindow(), "Failed to start the dialog.", fileName, MB_OK | MB_ICONERROR);
	}
	return EXIT_SUCCESS;
}


