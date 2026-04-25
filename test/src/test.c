#include <windows.h>
#include "beacon.h"

void go ( char * args, int len )
{
    datap parser;
    BeaconDataParse ( &parser, args, len );

    char * message = BeaconDataExtract ( &parser, NULL );

    BeaconPrintf ( CALLBACK_OUTPUT, "%s\n", message );
}
