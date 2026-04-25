x86:
    load $COFF "whoami.x86.o"
    pack $ARGS "b" "0x0"
    setg "%entrypoint" "_go"

    setg "%hooks" "hooks.spec"
    resolve "%hooks"

    before "export": run %hooks

x64:
    load $COFF "whoami.x64.o"
    pack $ARGS "b" "0x0"
    setg "%entrypoint" "go"

    setg "%hooks" "hooks.spec"
    resolve "%hooks"

    before "export": run %hooks
