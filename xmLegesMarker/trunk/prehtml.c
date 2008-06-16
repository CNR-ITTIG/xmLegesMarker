/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		prehtml.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "prehtml.h"
#include <string.h>
#include <stdio.h>
#include <IttigLogger.h>


static char *	bufferhtml = NULL;
static size_t	htmlsize;
static int pos = 0;

/******************************************************************************/
/**************************************************** PREHTML APPEND STRING ***/
/******************************************************************************/
void prehtmlAppendString(char *s) 
{
	int len = strlen(s);
	if (len + pos <= htmlsize) {
		memcpy(bufferhtml + pos, s, len);
		pos += len;
	}
}



/******************************************************************************/
/***************************************************** PREHTML APPEND CHARS ***/
/******************************************************************************/
void prehtmlAppendChars(int n, int c) 
{
	register int i;
	if (n + pos <= htmlsize) {
		for (i = 0; i < n; i++)
			*(bufferhtml + pos + i) = c;
		pos += n;
	}
}


void prehtmlInit(size_t s) 
{
	bufferhtml = (char *) malloc(sizeof(char) * s);
	htmlsize = s;
	pos = 0;
}

/******************************************************************************/
/********************************************************* PREHTML ANALIZZA ***/
/******************************************************************************/
char *prehtmlAnalizza(char *testo)
{
	loggerInfo("INIZIO prehtmlanalizza");
	prehtmlInit(strlen(testo)+1);
	//puts("-------------DOPO PREHTMLINIT-----------------------");
	prehtml_scan_bytes(testo, htmlsize);
	//puts("-------------DOPO SCAN_BYTES-----------------------");
	azzera();
	//puts("-------------DOPO AZZERA-----------------------");
	prehtmllex();
	//puts("-------------DOPO PREHTMLLEX-----------------------");
	loggerInfo("FINE prehtmlanalizza");
	return bufferhtml;
}

char prehtmlExtractChar(int elemento)
{
	
	char elementChar;
	int modLet;
	
	modLet = elemento % 26;
	if (modLet==0) 
		elementChar='z';
	else
		elementChar='a'+(modLet-1);
		
	return(elementChar);
}

