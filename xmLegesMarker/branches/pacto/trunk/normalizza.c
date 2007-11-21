/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		normalizza.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "normalizza.h"

static char *	buffer = NULL;
static size_t	size;
static int pos = 0;


/******************************************************************************/
/********************************************************** NORMALIZZA INIT ***/
/******************************************************************************/
void normalizzaInit(size_t s) {
	loggerInfo("INIZIO normalizzaInit");
	buffer = (char *) malloc(sizeof(char) * s);
	size = s;
	pos = 0;
	loggerInfo("FINE normalizzaInit");
}



/******************************************************************************/
/*************************************************************** NORMALIZZA ***/
/******************************************************************************/
char *normalizza(char *testo) {
	loggerInfo("INIZIO normalizza");
	normalizzaInit(strlen(testo)+1);
	nor_scan_bytes(testo, size);
	norlex();
	loggerInfo("FINE normalizza");
	return buffer;
}



/******************************************************************************/
/******************************************************** NOR APPEND STRING ***/
/******************************************************************************/
void norAppendString(char *s) {
	int len = strlen(s);
	if (len + pos <= size) {
		memcpy(buffer + pos, s, len);
		pos += len;
	}
	//loggerDebug("norAppendString OK");
}



/******************************************************************************/
/********************************************************* NOR APPEND CHARS ***/
/******************************************************************************/
void norAppendChars(int n, int c) {
	register int i;
	if (n + pos <= size) {
		for (i = 0; i < n; i++)
			*(buffer + pos + i) = c;
		pos += n;
	}
	//loggerDebug("norAppendChars OK");
}
