/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		pre.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "pre.h"

static char *	buffer = NULL;
static size_t	size;
static int pos = 0;



/******************************************************************************/
/***************************************************************** PRE INIT ***/
/******************************************************************************/
void preInit(size_t s) {
	buffer = (char *) malloc(sizeof(char) * s);
	size = s;
	pos = 0;
}



/******************************************************************************/
/************************************************************* PRE ANALIZZA ***/
/******************************************************************************/
char *preAnalizza(char *testo) {
	preInit(strlen(testo)+1);
	pre_scan_bytes(testo, size);
	prelex();
	//preparse();
	return buffer;
}



/******************************************************************************/
/******************************************************** PRE APPEND STRING ***/
/******************************************************************************/
void preAppendString(char *s) {
	int len = strlen(s);
	if (len + pos <= size) {
		memcpy(buffer + pos, s, len);
		pos += len;
	}
}



/******************************************************************************/
/********************************************************* PRE APPEND CHARS ***/
/******************************************************************************/
void preAppendChars(int n, int c) {
	register int i;
	if (n + pos <= size) {
		for (i = 0; i < n; i++)
			*(buffer + pos + i) = c;
		pos += n;
	}
}
