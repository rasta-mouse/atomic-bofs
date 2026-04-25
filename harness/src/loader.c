#include <windows.h>
#include "loader.h"
#include "tcg.h"
 
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$VirtualAlloc   ( LPVOID, SIZE_T, DWORD, DWORD );
DECLSPEC_IMPORT BOOL   WINAPI KERNEL32$VirtualProtect ( LPVOID, SIZE_T, DWORD, PDWORD );

char _COFF_ [ 0 ] __attribute__ ( ( section ( "coff" ) ) );
char _ARGS_ [ 0 ] __attribute__ ( ( section ( "args" ) ) );
 
void go ( )
{
    RESOURCE * src  = GETRESOURCE ( _COFF_ );

    char * code = KERNEL32$VirtualAlloc ( NULL, PicoCodeSize ( src->val ), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );
    char * data = KERNEL32$VirtualAlloc ( NULL, PicoDataSize ( src->val ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
 
    IMPORTFUNCS funcs;
    funcs.GetProcAddress = GetProcAddress;
    funcs.LoadLibraryA   = LoadLibraryA;

    PicoLoad ( ( IMPORTFUNCS * ) &funcs, src->val, code, data );

    RESOURCE * args = GETRESOURCE ( _ARGS_ );

    ( ( BOFMAIN ) PicoEntryPoint ( src->val, code ) ) ( args->val, args->len );
}
