#include <windows.h>

DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$VirtualAlloc ( LPVOID, SIZE_T, DWORD, DWORD );
DECLSPEC_IMPORT BOOL   WINAPI KERNEL32$VirtualFree  ( LPVOID, SIZE_T, DWORD );

HANDLE _GetCurrentProcess ( )
{
    /* return pseudo handle directly */
    return ( HANDLE ) ( -1 );
}
