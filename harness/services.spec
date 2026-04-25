x86:
    load "bin/services.x86.o"
        merge

    fixptrs "_caller"
    fixbss "_getBSS"
 
    dfr "_resolve" "ror13" "KERNEL32, NTDLL"
    dfr "_resolve_ext" "strings"
 
x64:
    load "bin/services.x64.o"
        merge
 
    fixbss "getBSS"
 
    dfr "resolve" "ror13" "KERNEL32, NTDLL"
    dfr "resolve_ext" "strings"
