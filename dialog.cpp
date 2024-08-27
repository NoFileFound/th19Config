#include "dialog.h"
#include "winapi.h"
#include <Commctrl.h>

// Buttons
CONST UINT BUTTON_SAVE = 1;
CONST UINT BUTTON_CANCEL = 1002;
CONST UINT BUTTON_RESET = 1054;

CONST UINT HOLD_SHOT_BUTTON_FOR_SLOW = 1036;
CONST UINT WINDOW_BORDERLESS_DOT_BY_DOT = 207;
CONST UINT WINDOW_BORDERLESS = 208;
CONST UINT WINDOW_FULLSCREEN_1280_960 = 209;
CONST UINT WINDOW_FULLSCREEN_960_720 = 210;
CONST UINT WINDOW_FULLSCREEN_640_480 = 211;
CONST UINT WINDOW_2560_1920 = 212;
CONST UINT WINDOW_1920_1440 = 213;
CONST UINT WINDOW_1280_960 = 214;
CONST UINT WINDOW_960_720 = 215;
CONST UINT WINDOW_640_480 = 216;
CONST UINT ASK_EACH_TIME = 202;
CONST UINT COLOR_FORMAT_32BITS = 1020;
CONST UINT COLOR_FORMAT_16BITS = 1021;
CONST UINT FRAME_SKIP_0 = 1017;
CONST UINT FRAME_SKIP_1 = 1018;
CONST UINT FRAME_SKIP_2 = 1019;
CONST UINT INPUT_MODE_SAFE = 1046;
CONST UINT INPUT_MODE_STANDARD = 1047;
CONST UINT INPUT_MODE_AUTODETECT = 1048;
CONST UINT INPUT_MODE_FAST = 1049;
CONST UINT JOYSTICK_DEAD_ZONE = 1052;

// OPCODES
CONST BYTE OPCODE_COLORFORMAT = 196;
CONST BYTE OPCODE_DISPLAYMODE = 199;
CONST BYTE OPCODE_SKIPPEDFRAMES = 200;
CONST BYTE OPCODE_INPUTMODE = 205;
CONST BYTE OPCODE_HOLDZASKEACHTIME = 209;

BYTE displayMode = 8;
BYTE holdZ = 0;
BYTE askEachTime = 1;
BYTE use16bitsColorFormat = 0;
BYTE skippedFrames = 0;
BYTE inputMode = 2;
INT joystickDeadZone = 600;

INT_PTR WINAPI 
DialogFunc(
	HWND hDialog, 
	UINT message, 
	WPARAM wParam, 
	LPARAM lParam
) {
	switch (message) {
		case WM_INITDIALOG:
			InitFileSettings(hDialog, "th19.cfg");
			SetTimer(hDialog, 0x7FFF, 10, NULL);
			break;

		case WM_HSCROLL:
			joystickDeadZone = (INT)SendMessageA(GetDlgItem(hDialog, JOYSTICK_DEAD_ZONE), TBM_GETPOS, FALSE, FALSE);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case BUTTON_SAVE:
					if (IsDlgButtonChecked(hDialog, FRAME_SKIP_0)) {
						skippedFrames = 0;
					}
					else {
						skippedFrames = (IsDlgButtonChecked(hDialog, FRAME_SKIP_1) != 1) + 1;
					}
					use16bitsColorFormat = (IsDlgButtonChecked(hDialog, COLOR_FORMAT_32BITS) != 1);
					askEachTime = IsDlgButtonChecked(hDialog, ASK_EACH_TIME);
					holdZ = IsDlgButtonChecked(hDialog, HOLD_SHOT_BUTTON_FOR_SLOW);
					if (IsDlgButtonChecked(hDialog, WINDOW_FULLSCREEN_640_480)) {
						displayMode = 0;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_FULLSCREEN_960_720)) {
						displayMode = 1;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_FULLSCREEN_1280_960)) {
						displayMode = 2;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_640_480)) {
						displayMode = 3;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_960_720)) {
						displayMode = 4;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_1280_960)) {
						displayMode = 5;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_1920_1440)) {
						displayMode = 6;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_2560_1920)) {
						displayMode = 7;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_BORDERLESS_DOT_BY_DOT)) {
						displayMode = 8;
					}
					else if (IsDlgButtonChecked(hDialog, WINDOW_BORDERLESS)) {
						displayMode = 9;
					}

					if (IsDlgButtonChecked(hDialog, INPUT_MODE_SAFE)) {
						inputMode = 0;
					}
					else if (IsDlgButtonChecked(hDialog, INPUT_MODE_STANDARD))
					{
						inputMode = 1;

					}
					else if (IsDlgButtonChecked(hDialog, INPUT_MODE_AUTODETECT)) {
						inputMode = 2;
					}
					else if (IsDlgButtonChecked(hDialog, INPUT_MODE_FAST)) {
						inputMode = 3;
					}

					SaveFileSettings();
					MessageBox(NULL, L"ä¬ã´ÇèëÇ´ä∑Ç¶Ç‹ÇµÇΩ", L"ämîF", MB_ICONINFORMATION);
					EndDialog(hDialog, TRUE);
					break;

				case BUTTON_CANCEL:
					EndDialog(hDialog, TRUE);
					break;

				case BUTTON_RESET:
					ResetFileSettings(hDialog);
					break;
			}
			break;

		case WM_TIMER:
			KillTimer(hDialog, 0x7FFF);
			SetTimer(hDialog, 0x7FFF, 10, NULL);
			break;

		case WM_CLOSE:
			KillTimer(hDialog, 0x7FFF);
			EndDialog(hDialog, TRUE);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

int ConvertBytesToInt(CONST BYTE byte1, CONST BYTE byte2) {
	return (byte2 << 8) + byte1;
}

BYTE getHoldZAskEachTimeByte(BOOL holdZ, BOOL askEachTime) {
	if (!holdZ) {
		if (!askEachTime) return 0x00;
		else return 0x01;
	}
	else {
		if (!askEachTime) return 0x02;
		return 0x03;
	}
}

VOID
SetConfigOptions(
	HWND hDlg,
	PVOID ConfigBuffer
)
{
	BYTE* configBuffer = static_cast<BYTE*>(ConfigBuffer);


	BYTE DISPLAYMODE = configBuffer[OPCODE_DISPLAYMODE];
	BYTE holdZ = -1;
	BYTE askEachTime = -1;
	BYTE use16bitsColorFormat = configBuffer[OPCODE_COLORFORMAT];
	BYTE skippedFrames = configBuffer[OPCODE_SKIPPEDFRAMES];
	BYTE inputMode = configBuffer[OPCODE_INPUTMODE];
	INT joystickDeadZone = ConvertBytesToInt(configBuffer[192], configBuffer[193]);

	// hold z and ask each time.
	switch (configBuffer[OPCODE_HOLDZASKEACHTIME]) {
		case 0x00:
			holdZ = 0;
			askEachTime = 0;
			break;
		case 0x01:
			holdZ = 0;
			askEachTime = 1;
			break;
		case 0x02:
			holdZ = 1;
			askEachTime = 0;
			break;
		case 0x03:
			holdZ = 1;
			askEachTime = 1;
			break;
		default:
			holdZ = 0;
			askEachTime = 0;
			break;
	}

	// hold z for slow mode.
	SendMessageA(GetDlgItem(hDlg, HOLD_SHOT_BUTTON_FOR_SLOW), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, HOLD_SHOT_BUTTON_FOR_SLOW), BM_SETCHECK, holdZ, FALSE);
	
	// display mode
	SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_640_480), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_960_720), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_1280_960), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_640_480), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_960_720), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_1280_960), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_1920_1440), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_2560_1920), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_BORDERLESS_DOT_BY_DOT), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, WINDOW_BORDERLESS), BM_SETCHECK, 0, FALSE);
	switch (DISPLAYMODE) {
		case 0:
			SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_640_480), BM_SETCHECK, 1, FALSE);
			break;
		case 1:
			SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_960_720), BM_SETCHECK, 1, FALSE);
			break;
		case 2:
			SendMessageA(GetDlgItem(hDlg, WINDOW_FULLSCREEN_1280_960), BM_SETCHECK, 1, FALSE);
			break;
		case 3:
			SendMessageA(GetDlgItem(hDlg, WINDOW_640_480), BM_SETCHECK, 1, FALSE);
			break;
		case 4:
			SendMessageA(GetDlgItem(hDlg, WINDOW_960_720), BM_SETCHECK, 1, FALSE);
			break;
		case 5:
			SendMessageA(GetDlgItem(hDlg, WINDOW_1280_960), BM_SETCHECK, 1, FALSE);
			break;
		case 6:
			SendMessageA(GetDlgItem(hDlg, WINDOW_1920_1440), BM_SETCHECK, 1, FALSE);
			break;
		case 7:
			SendMessageA(GetDlgItem(hDlg, WINDOW_2560_1920), BM_SETCHECK, 1, FALSE);
			break;
		case 8:
			SendMessageA(GetDlgItem(hDlg, WINDOW_BORDERLESS_DOT_BY_DOT), BM_SETCHECK, 1, FALSE);
			break;
		case 9:
			SendMessageA(GetDlgItem(hDlg, WINDOW_BORDERLESS), BM_SETCHECK, 1, FALSE);
			break;
	}

	// ask each time
	SendMessageA(GetDlgItem(hDlg, ASK_EACH_TIME), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, ASK_EACH_TIME), BM_SETCHECK, askEachTime, FALSE);

	// color format
	SendMessageA(GetDlgItem(hDlg, COLOR_FORMAT_16BITS), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, COLOR_FORMAT_32BITS), BM_SETCHECK, 0, FALSE);
	if (use16bitsColorFormat) {
		SendMessageA(GetDlgItem(hDlg, COLOR_FORMAT_16BITS), BM_SETCHECK, 1, FALSE);
	}
	else {
		SendMessageA(GetDlgItem(hDlg, COLOR_FORMAT_32BITS), BM_SETCHECK, 1, FALSE);
	}

	// frame skip
	SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_0), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_1), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_2), BM_SETCHECK, 0, FALSE);
	switch (skippedFrames) {
		case 1:
			SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_1), BM_SETCHECK, 1, FALSE);
			break;
		case 2:
			SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_2), BM_SETCHECK, 1, FALSE);
			break;
		default:
			SendMessageA(GetDlgItem(hDlg, FRAME_SKIP_0), BM_SETCHECK, 1, FALSE);
			break;
	}

	// input mode
	SendMessageA(GetDlgItem(hDlg, INPUT_MODE_SAFE), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, INPUT_MODE_STANDARD), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, INPUT_MODE_AUTODETECT), BM_SETCHECK, 0, FALSE);
	SendMessageA(GetDlgItem(hDlg, INPUT_MODE_FAST), BM_SETCHECK, 0, FALSE);
	switch (inputMode) {
		case 1:
			SendMessageA(GetDlgItem(hDlg, INPUT_MODE_STANDARD), BM_SETCHECK, 1, FALSE);
			break;
		case 2:
			SendMessageA(GetDlgItem(hDlg, INPUT_MODE_AUTODETECT), BM_SETCHECK, 1, FALSE);
			break;
		case 3:
			SendMessageA(GetDlgItem(hDlg, INPUT_MODE_FAST), BM_SETCHECK, 1, FALSE);
			break;
		default:
			SendMessageA(GetDlgItem(hDlg, INPUT_MODE_SAFE), BM_SETCHECK, 1, FALSE);
			break;
	}

	// Joystick dead zone
	SendMessageA(GetDlgItem(hDlg, JOYSTICK_DEAD_ZONE), TBM_SETRANGE, TRUE, MAKELPARAM(0, 1000));
	SendMessageA(GetDlgItem(hDlg, JOYSTICK_DEAD_ZONE), TBM_SETPOS, TRUE, joystickDeadZone);
}

PVOID
SaveConfigOptions(
	PVOID ConfigBuffer
)
{
	BYTE* configBuffer = static_cast<BYTE*>(ConfigBuffer);
	configBuffer[OPCODE_DISPLAYMODE] = displayMode;
	configBuffer[OPCODE_SKIPPEDFRAMES] = skippedFrames;
	configBuffer[OPCODE_INPUTMODE] = inputMode;
	configBuffer[OPCODE_HOLDZASKEACHTIME] = getHoldZAskEachTimeByte(holdZ, askEachTime);
	configBuffer[OPCODE_COLORFORMAT] = use16bitsColorFormat;

	configBuffer[192] = joystickDeadZone & 0xFF;
	configBuffer[193] = (joystickDeadZone >> 8) & 0xFF;

	configBuffer[194] = configBuffer[192];
	configBuffer[195] = configBuffer[193];

	return configBuffer;
}