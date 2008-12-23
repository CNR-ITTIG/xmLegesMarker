/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		prexml.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
#include "prexml.h"
#include <string.h>
#include <stdio.h>
#include <IttigLogger.h>


static char *	bufferxml = NULL;
static size_t	xmlsize;
static int pos = 0;

/******************************************************************************/
/**************************************************** PRExml APPEND STRING ***/
/******************************************************************************/
void prexmlAppendString(char *s) 
{
	int len = strlen(s);
	if (len + pos <= xmlsize) {
		memcpy(bufferxml + pos, s, len);
		pos += len;
	}
}



/******************************************************************************/
/***************************************************** PRExml APPEND CHARS ***/
/******************************************************************************/
void prexmlAppendChars(int n, int c) 
{
	register int i;
	if (n + pos <= xmlsize) {
		for (i = 0; i < n; i++)
			*(bufferxml + pos + i) = c;
		pos += n;
	}
}


void prexmlInit(size_t s) 
{
	bufferxml = (char *) malloc(sizeof(char) * s);
	xmlsize = s;
	pos = 0;
}

/******************************************************************************/
/********************************************************* PRExml ANALIZZA ***/
/******************************************************************************/
char *prexmlAnalizza(char *testo)
{
	loggerInfo("INIZIO prexmlanalizza");
	prexmlInit(strlen(testo)+1);
	//puts("-------------DOPO PRExmlINIT-----------------------");
	prexml_scan_bytes(testo, xmlsize);
	//puts("-------------DOPO SCAN_BYTES-----------------------");
	//azzera();
	//puts("-------------DOPO AZZERA-----------------------");
	prexmllex();
	//puts("-------------DOPO PRExmlLEX-----------------------");
	loggerInfo("FINE prexmlanalizza");
	return bufferxml;
}

char prexmlExtractChar(int elemento)
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

