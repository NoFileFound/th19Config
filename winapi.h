#ifndef __WINAPI_H
#define __WINAPI_H
#include<Windows.h>

#define LOCALE_JAPAN_LANG 0x0411
#define LOCALE_BRAZILIAN_LANG 0x0416

VOID
GetCurrentProgramName(
    PCHAR fileName,
    DWORD bufferSize
);

VOID
InitFileSettings(
    HWND hDialog,
    LPCSTR lpFileName
);

VOID
ResetFileSettings(
    HWND hDialog
);

VOID
SaveFileSettings(

);

#endif // __WINAPI_H