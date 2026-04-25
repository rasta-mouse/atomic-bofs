x86:
    load "bin/loader.x86.o"
        make pic +gofirst +optimize
        run "services.spec"
        mergelib "libtcg.x86.zip"
 
        push $COFF
            make object +optimize
            run "bof.spec" %entrypoint
            export
            link "bof"
        
        push $ARGS
            preplen
            link "args"

    export
 
x64:
    load "bin/loader.x64.o"
        make pic +gofirst +optimize
        run "services.spec"
        mergelib "libtcg.x64.zip"
 
        push $COFF
            make object +optimize
            run "bof.spec" %entrypoint
            export
            preplen
            link "coff"
        
        push $ARGS
            preplen
            link "args"

    export
