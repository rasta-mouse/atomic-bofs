/*
 * Copyright 2026 Raphael Mudge, Adversary Fan Fiction Writers Guild
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS Ã¢Â€ÂœAS ISÃ¢Â€Â AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <windows.h>
 
DECLSPEC_IMPORT void   WINAPIV MSVCRT$free      ( void * );
DECLSPEC_IMPORT void * WINAPIV MSVCRT$malloc    ( size_t );
DECLSPEC_IMPORT int    WINAPIV MSVCRT$vsnprintf ( char *, size_t, const char *, va_list );
DECLSPEC_IMPORT int    WINAPIV MSVCRT$printf    ( const char *, ... );
 
#define memset(x, y, z) __stosb ( ( unsigned char *) x, y, z );
#define memcpy(x, y, z) __movsb ( ( unsigned char *) x, ( unsigned char * ) y, z );
 
/* format API */
typedef struct {
    char * original; /* the original buffer [so we can free it] */
    char * buffer;   /* current pointer into our buffer */
    int    length;   /* remaining length of data */
    int    size;     /* total size of this buffer */
} formatp, datap;
 
int    BeaconDataInt     ( datap * );
short  BeaconDataShort   ( datap * );
char * BeaconDataExtract ( datap *, int * );
int    BeaconDataLength  ( datap * );
void   BeaconDataParse   ( datap *, char *, int );
char * BeaconDataPtr     ( datap *, int );
 
void   BeaconFormatAlloc    ( formatp *, int );
void   BeaconFormatReset    ( formatp * );
void   BeaconFormatAppend   ( formatp *, char *, int );
void   BeaconFormatPrintf   ( formatp *, char *, ...);
char * BeaconFormatToString ( formatp *, int * );
void   BeaconFormatFree     ( formatp * );
void   BeaconFormatInt      ( formatp *, int );
 
void   BeaconPrintf ( int, char *, ...);
void   BeaconOutput ( int, char *, int );
 
char * BofApiAlloc ( SIZE_T );
void   BofApiFree  ( char * );
 
int BeaconDataLength ( datap * parser )
{
    return parser->length;
}
 
int BeaconDataInt( datap * parser )
{
    int * foo = ( int * ) BeaconDataPtr ( parser, sizeof ( int ) );
 
    if ( foo == NULL )
        return 0;
 
    return * foo;
}
 
short BeaconDataShort ( datap * parser )
{
    short * foo = ( short * ) BeaconDataPtr ( parser, sizeof ( short ) );
 
    if ( foo == NULL )
        return 0;
 
    return * foo;
}
 
char * BeaconDataExtract ( datap * parser, int * size )
{
    int    len = BeaconDataInt ( parser );
    char * res = BeaconDataPtr ( parser, len );
 
    if ( size != NULL )
        *size = ( res == NULL ) ? 0 : len;
 
    return res;
}
 
void BeaconDataParse ( datap * parser, char * buffer, int size )
{
    parser->original = buffer;
    parser->buffer   = buffer;
    parser->length   = size;
    parser->size     = size;
}
 
char * BeaconDataPtr ( datap * parser, int size )
{
    char * temp = parser->buffer;
 
    /* don't give out any more pointers if something goes wrong */
    if ( size < 0 || size > parser->length )
    {
        parser->length = 0;
        return NULL;
    }
 
    parser->buffer += size;
    parser->length -= size;
    
    return temp;
}
 
void BeaconFormatAlloc ( formatp * format, int maxsz )
{
    format->original = BofApiAlloc ( maxsz );
    format->buffer   = format->original;
    format->length   = 0;
    format->size     = maxsz;
}
 
void BeaconFormatReset ( formatp * format )
{
    format->buffer = format->original;
    format->length = 0;

    memset ( format->original, 0, format->size );
}
 
void BeaconFormatAppend ( formatp * format, char * text, int len )
{
    if ( format->length >= format->size )
        return;
 
    /* we're going to truncate if there's not enough space */
    int remaining = format->size - format->length;
    int writelen  = ( remaining < len ) ? remaining : len;
 
    memcpy ( format->buffer, text, writelen );

    format->length += writelen;
    format->buffer += writelen;
}
 
void BeaconFormatPrintf ( formatp * format, char * fmt, ... )
{
    int    len;
    char * temp;
 
    va_list args;
 
    /* figure out the length of our buffer */
    va_start ( args, fmt );
    len  = MSVCRT$vsnprintf ( NULL, 0, fmt, args );
    va_end ( args );
 
    if ( len < 0 )
        return;
 
    /* allocate our memory */
    temp = BofApiAlloc ( len + 1 );
    
    if ( temp == NULL )
        return;
 
    /* format everything */
    va_start ( args, fmt );
    MSVCRT$vsnprintf ( temp, len + 1, fmt, args );
    va_end ( args );
 
    /* print it */
    BeaconFormatAppend ( format, temp, len );
 
    /* free our memory and move on with our lives */
    BofApiFree ( temp );
}
 
char * BeaconFormatToString ( formatp * format, int * size )
{
    if ( size != NULL )
        *size = format->length;
 
    return format->original;
}
 
void BeaconFormatFree ( formatp * format )
{
    BofApiFree ( format->original );
 
    format->buffer   = NULL;
    format->original = NULL;
    format->length   = 0;
    format->size     = 0;
}
 
void BeaconFormatInt ( formatp * format, int value )
{
    BeaconFormatAppend ( format, ( char * ) &value, sizeof ( int ) );
}
 
void BeaconPrintf ( int type, char * format, ... )
{
    int    len;
    char * temp;
 
    va_list args;
 
    /* figure out the length of our buffer */
    va_start ( args, format );
    len  = MSVCRT$vsnprintf ( NULL, 0, format, args );
    va_end ( args );
 
    if ( len < 0 )
        return;
 
    /* allocate our memory */
    temp = BofApiAlloc ( len + 1 );
 
    if ( temp == NULL )
        return;
 
    /* format everything */
    va_start ( args, format );
    MSVCRT$vsnprintf ( temp, len + 1, format, args );
    va_end(args);
 
    /* print it */
    BeaconOutput ( type, temp, len );
 
    /* free our memory and move on with our lives */
    BofApiFree ( temp );
}

void BeaconOutput ( int type, char * data, int len )
{
    MSVCRT$printf ( "BeaconOutput[%d]: %s", type, data );
}
 
/* This is a potential redirect target if you want to change how this API allocates RW memory */
char * BofApiAlloc ( SIZE_T size )
{
    return MSVCRT$malloc ( size );
}
 
/* Same, a potential redirect target to change how this API frees RW memory */
void BofApiFree ( char * ptr )
{
    MSVCRT$free ( ptr );
}