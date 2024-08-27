#include "winapi.h"
#include "dialog.h"
#include<stdio.h>

#define DEFAULTFILE_SIZE 256
const unsigned char DefaultFile[DEFAULTFILE_SIZE] = {
    0x06, 0x00, 0x19, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x05, 0x00, 0x0A, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5A, 0x00, 0x58, 0x00, 0x43, 0x00, 0x10, 0x00,
    0x1B, 0x00, 0x26, 0x00, 0x28, 0x00, 0x25, 0x00, 0x27, 0x00, 0x5A, 0x00, 0x58, 0x00, 0x43, 0x00,
    0x10, 0x00, 0x1B, 0x00, 0x47, 0x00, 0x42, 0x00, 0x56, 0x00, 0x4E, 0x00, 0x28, 0x00, 0x27, 0x00,
    0x26, 0x00, 0x25, 0x00, 0x1B, 0x00, 0x65, 0x00, 0x62, 0x00, 0x61, 0x00, 0x63, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x05, 0x00, 0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x5A, 0x00, 0x58, 0x00, 0x43, 0x00, 0x10, 0x00, 0x1B, 0x00, 0x26, 0x00, 0x28, 0x00,
    0x25, 0x00, 0x27, 0x00, 0x5A, 0x00, 0x58, 0x00, 0x43, 0x00, 0x10, 0x00, 0x1B, 0x00, 0x47, 0x00,
    0x42, 0x00, 0x56, 0x00, 0x4E, 0x00, 0x28, 0x00, 0x27, 0x00, 0x26, 0x00, 0x25, 0x00, 0x1B, 0x00,
    0x65, 0x00, 0x62, 0x00, 0x61, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x58, 0x02, 0x58, 0x02, 0x00, 0x01, 0x01, 0x08, 0x00, 0x02, 0x64, 0x50, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x54, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

VOID 
GetCurrentProgramName(
    PCHAR fileName, 
    DWORD bufferSize
) {
    if (GetModuleFileNameA(NULL, fileName, bufferSize) > 0) {
        PCHAR lastBackslash = strrchr(fileName, '\\');
        if (lastBackslash != NULL) {
            memmove(fileName, lastBackslash + 1, strlen(lastBackslash));
        }
    }
    else {
        strcpy(fileName, "custom.exe");
    }
}

PVOID
ZunReadFile(
    LPCSTR lpFileName,
    DWORD* fileSize
) {
    HANDLE hFile;
    DWORD nNumberOfBytesToRead;
    PVOID Buffer;

    printf("%s Load ... \r\n", lpFileName);
    hFile = CreateFileA(
        lpFileName, 
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("error : %s is not found.\r\n", lpFileName);
        return NULL;
    }
    nNumberOfBytesToRead = GetFileSize(hFile, 0);
    Buffer = malloc(nNumberOfBytesToRead);
    if (!Buffer) {
        printf("error : %s allocation error.\r\n", lpFileName);
        CloseHandle(hFile);
        return NULL;
    }
    ReadFile(hFile, Buffer, nNumberOfBytesToRead, &nNumberOfBytesToRead, NULL);
    if (fileSize) *fileSize = nNumberOfBytesToRead;
    CloseHandle(hFile);
    return Buffer;
}

VOID
ResetFileSettings(
    HWND hDialog
)
{
    PVOID ConfigBuffer = malloc(DEFAULTFILE_SIZE);
    if (!ConfigBuffer) {
        printf("allocation error");
        exit(EXIT_FAILURE);
    }
    memcpy(ConfigBuffer, DefaultFile, DEFAULTFILE_SIZE);
    SetConfigOptions(hDialog, ConfigBuffer);
}

VOID
InitFileSettings(
    HWND hDialog,
    LPCSTR lpFileName
)
{
    CHAR Directory[4096];
    PVOID ConfigBuffer;
    DWORD ConfigBufferSize;

    memset(Directory, 0, sizeof(Directory));
    DWORD exist = GetEnvironmentVariableA("APPDATA", Directory, 1000);
    if (exist) {
        strcat_s(Directory, sizeof(Directory), "\\ShanghaiAlice");
        CreateDirectoryA(Directory, NULL);

        strcat_s(Directory, sizeof(Directory), "\\th19");
        CreateDirectoryA(Directory, NULL);

        strcat_s(Directory, sizeof(Directory), "\\");
    }
    SetCurrentDirectoryA(Directory);
    ConfigBuffer = ZunReadFile(lpFileName, &ConfigBufferSize);
    if (!ConfigBuffer) {
        ConfigBuffer = malloc(DEFAULTFILE_SIZE);
        if (!ConfigBuffer) {
            printf("allocation error");
            exit(EXIT_FAILURE);
        }
        memcpy(ConfigBuffer, DefaultFile, DEFAULTFILE_SIZE);
    }
    SetConfigOptions(hDialog, ConfigBuffer);
}

VOID
ZunSaveFile(
    LPCSTR lpFileName,
    PVOID Buffer,
    DWORD fileSize
)
{
    HANDLE fileHandle = CreateFileA(
        lpFileName,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (fileHandle == INVALID_HANDLE_VALUE) {
        DWORD lastError = GetLastError();
        char errorMsg[512];

        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            lastError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMsg,
            sizeof(errorMsg),
            NULL
        );
        LocalFree(Buffer);
        return;
    }

    DWORD bytesWritten;
    WriteFile(fileHandle, Buffer, fileSize, &bytesWritten, NULL);
    CloseHandle(fileHandle);
}

VOID
SaveFileSettings(

)
{
    PVOID ConfigBuffer = malloc(DEFAULTFILE_SIZE);
    if (!ConfigBuffer) {
        printf("allocation error");
        exit(EXIT_FAILURE);
    }
    memcpy(ConfigBuffer, DefaultFile, DEFAULTFILE_SIZE);
    ConfigBuffer = SaveConfigOptions(ConfigBuffer);
    ZunSaveFile("th19.cfg", ConfigBuffer, DEFAULTFILE_SIZE);
}