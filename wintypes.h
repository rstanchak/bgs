#ifndef WINTYPES_H
#define WINTYPES_H

#include <stdlib.h>
#include <stdio.h>

typedef int             BOOL,       *PBOOL,    *LPBOOL;
typedef unsigned char   BYTE,       *PBYTE,    *LPBYTE;
typedef unsigned char   UCHAR,      *PUCHAR;

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0 

void AfxMessageBox(const char * msg){
	fprintf(stderr, "%s\n", msg);
}

#endif
