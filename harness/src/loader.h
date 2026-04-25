typedef struct {
    int  len;
    char val [ ];
} RESOURCE;

#define GETRESOURCE(x) ( RESOURCE * ) &x
 
typedef void ( * BOFMAIN ) ( char *, int );