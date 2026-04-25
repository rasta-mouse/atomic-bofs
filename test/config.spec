x86:
    load $COFF "bin/test.x86.o"
    pack $ARGS "iz" "16" "Hello World x86"
    setg "%entrypoint" "_go"

x64:
    # load your coff
    load $COFF "bin/test.x64.o"

    # pack args
    pack $ARGS "iz" "16" "Hello World x64"
    
    # if none, pack 0
    # pack $ARGS "b" "0x0"

    # set desired entry point
    setg "%entrypoint" "go"
