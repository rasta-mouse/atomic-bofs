x86:
    load "../bin/hooks.x86.o"
        merge
    
    attach "KERNEL32$GetCurrentProcess" "__GetCurrentProcess"

x64:
    load "../bin/hooks.x64.o"
        merge
    
    attach "KERNEL32$GetCurrentProcess" "_GetCurrentProcess"
